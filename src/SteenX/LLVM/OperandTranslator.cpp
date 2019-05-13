#include "SteenX/LLVM/OperandTranslator.h"

#include "llvm/IR/GlobalVariable.h"

#include "SteenX/LLVM/GetElementPtrExtraction.h"
#include "SteenX/Program/Instruction/ReferenceInstruction.h"
#include "SteenX/Program/Operand/FunctionOperand.h"
#include "SteenX/Program/Operand/LocalMemoryAtomOperand.h"

bool
racehog::OperandTranslator::Translation::
preload(Function&                             func,
        const Instruction::MetadataReference& meta) {
  if (isConstantOperand() == true)
    return true; // Fast path.
  if (auto llvm_expr = llvm::dyn_cast<llvm::ConstantExpr>(llvm_val)) {
    if (llvm_expr->getOpcode() == llvm::Instruction::GetElementPtr) {
      GetElementPtrExtraction gep(optrans.sizes, llvm_expr);
      if (gep.getRangeRepeat() >  0    ||
          gep.getRangeSpread() >  0    ||
          gep.hasMoreIndices() == true) {
        // This error should really never fire in well-formed LLVM programs:
        llvm::errs() << "Unexpected range index in: " << *llvm_expr << "\n";
        return false;
      }
      auto  base = optrans.preload(func, meta, llvm_expr->getOperand(0));
      auto& ref  = func.append<ReferenceInstruction>(meta, gep.getBaseOffset());
      (void) ref.append<LocalMemoryAtomOperand>(
        optrans.locals.request(llvm_val));
      return base.appendAsPointerTo(ref);
    }
    return true;
  }
  return true;
}

bool
racehog::OperandTranslator::Translation::
appendToImpl(Instruction& inst,
             bool         is_ptr) {
  // Ordered roughly by expected ratio of occurance.

  // TODO:  Work on this list:
  //
  // [ ] Constant:
  //     [ ] BlockAddress
  //     [ ] ConstantAggregate:
  //         [ ] ConstantArray
  //         [ ] ConstantStruct
  //         [ ] ConstantVector
  //     [ ] ConstantData:
  //         [ ] ConstantAggregateZero
  //         [ ] ConstantDataSequential:
  //             [ ] ConstantDataArray
  //             [ ] ConstantDataVector
  //         [x] ConstantFP
  //         [x] ConstantInt
  //         [x] ConstantPointerNull
  //         [ ] ConstantTokenNone
  //         [ ] UndefValue
  //     [~] ConstantExpr
  //     [ ] GlobalValue:
  //         [ ] GlobalIndirectSymbol:
  //             [ ] GlobalAlias
  //             [ ] GlobalFunc
  //         [ ] GlobalObject:
  //             [x] Function
  //             [x] GlobalVariable
  if (optrans.locals.contains(llvm_val) == true) {
    (void) inst.append<LocalMemoryAtomOperand>(
      optrans.locals.lookup(llvm_val), is_ptr);
    return true;
  }
  if (optrans.globals.contains(llvm_val) == true) {
    (void) inst.append<GlobalMemoryAtomOperand>(
      optrans.globals.lookup(llvm_val), is_ptr);
    return true;
  }
  if (isConstantOperand() == true) {
    if (is_ptr == true) {
      LOG(ERROR) << "Cannot create pointer of ConstantOperand.";
      return false;
    }
    (void) inst.append<ConstantOperand>();
    return true;
  }
  if (const auto llvm_func = llvm::dyn_cast<llvm::Function>(llvm_val)) {
    if (is_ptr == true) {
      LOG(ERROR) << "Cannot create pointer of FunctionOperand.";
      return false;
    }
    (void) inst.append<FunctionOperand>(optrans.funcs.get(llvm_func));
    return true;
  }
  if (const auto llvm_expr = llvm::dyn_cast<llvm::ConstantExpr>(llvm_val)) {
    // TODO:  Deal with all other types as well.
    switch (llvm_expr->getOpcode()) {
    case llvm::Instruction::BitCast:
    case llvm::Instruction::GetElementPtr: {
      // Offset == 0 can be expected, all else would have been loaded into a
      // local memory atom by Translation::preload.  Simply forward the
      // expression's operand:
      Translation expr(optrans, llvm_expr->getOperand(0));
      if (expr.appendToImpl(inst, is_ptr) == false) {
        LOG(ERROR) << "Translation::appendToImpl failed";
        return false;
      }
      return true;
    }
    default:
      llvm::errs() << "Unexpected constant expression: " << *llvm_expr << "\n";
      return false;
    }
  }
  if (const auto llvm_var = llvm::dyn_cast<llvm::GlobalVariable>(llvm_val)) {
    // `appendToImpl()` will be able to resolve `llvm_val` after successful
    // initialisation.
    return optrans.ginit.initialise(llvm_var) == true ?
      appendToImpl(inst, is_ptr) : false;
  }
  llvm::errs() << "Unexpected value: " << *llvm_val << "\n";
  return false;
}
