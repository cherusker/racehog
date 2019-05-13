#include "SteenX/LLVM/InstructionExtractor.h"

#include "llvm/IR/Constant.h"

#include "SteenX/LLVM/GetElementPtrExtraction.h"
#include "SteenX/Program/Instruction/Instructions.h"
#include "Support/Macros.h"

void
racehog::InstructionExtractor::
visitInstruction(llvm::Instruction& inst) {
  LLVM_FATAL_EXIT(inst, "Unable to resolve instruction.");
}

void
racehog::InstructionExtractor::
visitAllocaInst(llvm::AllocaInst& llvm_inst) {
  auto num_atoms = sizes.getSizeOf(llvm_inst.getAllocatedType());
  if (llvm_inst.isArrayAllocation() == true) {
    if (const auto size =
        llvm::dyn_cast<llvm::ConstantInt>(llvm_inst.getArraySize()))
      num_atoms *= size->getZExtValue();
    else
      LLVM_FATAL_EXIT(llvm_inst, "Dynamic stack frame size.");
  }
  auto id = locals.request(&llvm_inst, num_atoms + 1);
  (void) locals.emplaceLocalReference(
    metax.extractFrom(llvm_inst), id, id + 1);
}

void
racehog::InstructionExtractor::
visitLoadInst(llvm::LoadInst& llvm_inst) {
  const auto meta = metax.extractFrom(llvm_inst);
  auto       ptr  = optrans.preload(func, meta, llvm_inst.getPointerOperand());
  auto&      eq   = func.append<EqualiseInstruction>(meta);
  (void) eq.append<LocalMemoryAtomOperand>(locals.request(&llvm_inst));
  if (ptr.appendAsPointerTo(eq) == false)
    LLVM_FATAL_EXIT(llvm_inst, "Translation::appendAsPointerTo failed.");
  (void) func.append<WriteInstruction>(meta, eq.getOperand(0));
  (void) func.append<ReadInstruction>(meta, eq.getOperand(1));
}

void
racehog::InstructionExtractor::
visitStoreInst(llvm::StoreInst& llvm_inst) {
  const auto meta = metax.extractFrom(llvm_inst);
  auto ptr = optrans.preload(func, meta, llvm_inst.getPointerOperand());
  auto val = optrans.preload(func, meta, llvm_inst.getValueOperand());
  if (val.isConstantOperand() == true) {
    auto& write = func.append<WriteInstruction>(meta);
    if (ptr.appendAsPointerTo(write) == false)
      LLVM_FATAL_EXIT(llvm_inst, "Translation::appendAsPointerTo failed.");
    return; // No need for the rest when storing primitive constants.
  }
  auto& eq = func.append<EqualiseInstruction>(meta);
  if (val.appendAsValueTo(eq) == false)
    LLVM_FATAL_EXIT(llvm_inst, "Translation::appendAsValueTo failed.");
  if (ptr.appendAsPointerTo(eq) == false)
    LLVM_FATAL_EXIT(llvm_inst, "Translation::appendAsPointerTo failed.");
  const auto& valop = eq.getOperand(0);
  if (valop.isMemoryAtom() == true) // Could also be a function pointer.
    (void) func.append<ReadInstruction>(meta, valop);
  (void) func.append<WriteInstruction>(meta, eq.getOperand(1));
}

void
racehog::InstructionExtractor::
visitGetElementPtrInst(llvm::GetElementPtrInst& llvm_inst) {
  const auto meta = metax.extractFrom(llvm_inst);
  // The thing about `base_id` and the weird logic in here ...
  //   We have to differentiate between three different cases that are part of
  //   _every_ GEP translation:
  //     1. First alias:  bufX -> llvm_base
  //     2. Middle alias_es_ (they can repeat):  bufY -> bufX
  //     3. Last alias:  llvm_inst -> bufY
  //   Now, these three cases can either come in 1, 2, or 3+ instruction
  //   (depending on the GEP's indices):
  //     1 instruction:  llvm_inst -> llvm_base
  //     2 instructions:  bufX -> llvm_base, llvm_inst -> bufX
  //     3 instructions: as seen above.
  //   To account for _all_ possible cases, we have to attach some extra logic:
  //     LHS: last_index  ? llvm_inst : bufX
  //     RHS: first_index ? llvm_base : bufX-1 (note the `-1` here!)
  bool                                 is_first    = true;
  LocalMemoryAtomOperand::MemoryAtomId base_id     = 0;
  LocalMemoryAtomOperand::MemoryAtomId new_base_id = 0;
  GetElementPtrExtraction gep(sizes, llvm_inst);
  auto base = optrans.preload(func, meta, llvm_inst.getOperand(0));
  do {
    auto& ref = func.append<ReferenceInstruction>(
      meta,
      gep.getBaseOffset(),
      gep.getRangeRepeat(),
      gep.getRangeSpread());
    new_base_id = ref.append<LocalMemoryAtomOperand>(
      locals.request(gep.hasMoreIndices() == true ? nullptr : &llvm_inst))
      .getMemoryAtomId();
    if (is_first == true) {
      if (base.appendAsPointerTo(ref) == false)
        LLVM_FATAL_EXIT(llvm_inst, "Translation::appendAsPointerTo failed.");
      is_first = false;
    } else {
      (void) ref.append<LocalMemoryAtomOperand>(base_id);
    }
    base_id = new_base_id;
  } while (gep.advance() == true);
}

void
racehog::InstructionExtractor::
visitPHINode(llvm::PHINode& llvm_inst) {
  // TODO:  Test thoroughly.
  const auto meta = metax.extractFrom(llvm_inst);
  for (const auto& llvm_val : llvm_inst.incoming_values()) {
    auto val = optrans.preload(func, meta, llvm_val.get());
    auto& eq = func.append<EqualiseInstruction>(meta);
    (void) eq.append<LocalMemoryAtomOperand>(locals.request(&llvm_inst));
    if (val.appendAsValueTo(eq) == false)
      LLVM_FATAL_EXIT(llvm_inst, "Translation::appendAsValueTo failed.");
  }
}

void
racehog::InstructionExtractor::
visitCallInst(llvm::CallInst& llvm_inst) {
  if (call_handlers.resolve(func, llvm_inst) == false)
    LLVM_FATAL_EXIT(llvm_inst, "CallHandlerRegistry::resolve failed.");
}

void
racehog::InstructionExtractor::
visitReturnInst(llvm::ReturnInst& llvm_inst) {
  const auto llvm_retval = llvm_inst.getReturnValue();
  if (llvm_retval == nullptr)
    return; // Void return values can be skipped.
  const auto meta = metax.extractFrom(llvm_inst);
  auto retval = optrans.preload(func, meta, llvm_retval);
  if (retval.isConstantOperand() == true)
    return; // Primitive constant return values can be skipped.
  auto& ret = func.append<ReturnInstruction>(meta);
  if (retval.appendAsValueTo(ret) == false)
    LLVM_FATAL_EXIT(llvm_inst, "Translation::appendAsValueTo failed.");
}

void
racehog::InstructionExtractor::
visitExtractValueInst(llvm::ExtractValueInst& llvm_inst) {
  // TODO:  Deal with whatever LLVM throws at us.
  // Right now we only accept integers and floats as they can be ignored.
  auto llvm_type = llvm_inst.getType();
  if (llvm_type->isIntegerTy()       == false &&
      llvm_type->isFloatingPointTy() == false)
    LLVM_FATAL_EXIT(llvm_inst, "Unable to resolve result type.");
  // TODO:  (void) locals.request...
}

void
racehog::InstructionExtractor::
visitAtomicInstImpl(llvm::Instruction&          llvm_inst,
                    llvm::Value*                llvm_ptr,
                    MemoryManager::MemoryAtomId num_retvals) {
  (void) locals.request(&llvm_inst, num_retvals);
  const auto meta    = metax.extractFrom(llvm_inst);
  const auto lock_id = atomics.getLockMemoryAtomId(llvm_ptr);
  auto& acq          = func.append<AcquireInstruction>(meta);
  (void) acq.append<GlobalMemoryAtomOperand>(lock_id, true);
  auto ptr = optrans.preload(func, meta, llvm_ptr);
  // `Write` is a semantic superset of `read` so the `read` part of atomicrmw
  // can be ignored while staying sound with respect to data races.
  auto& write = func.append<WriteInstruction>(meta);
  if (ptr.appendAsPointerTo(write) == false)
    LLVM_FATAL_EXIT(llvm_inst, "Translation::appendAsPointerTo failed.");
  auto& rel = func.append<ReleaseInstruction>(meta);
  (void) rel.append<GlobalMemoryAtomOperand>(lock_id, true);
}

void
racehog::InstructionExtractor::
visitSimpleCastInstImpl(llvm::CastInst& llvm_inst) {
  const auto meta = metax.extractFrom(llvm_inst);
  auto base       = optrans.preload(func, meta, llvm_inst.getOperand(0));
  // Casting primitive constants to another type can be skipped but does LLVM
  // even emit that?
  if (base.isConstantOperand() == true)
    return;
  auto& eq = func.append<EqualiseInstruction>(meta);
  (void) eq.append<LocalMemoryAtomOperand>(locals.request(&llvm_inst));
  if (base.appendAsValueTo(eq) == false)
    LLVM_FATAL_EXIT(llvm_inst, "Translation::appendAsPointerTo failed.");
}
