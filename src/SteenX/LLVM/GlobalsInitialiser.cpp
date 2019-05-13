#include "SteenX/LLVM/GlobalsInitialiser.h"

#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

#include "SteenX/Program/Instruction/EqualiseInstruction.h"

bool
racehog::GlobalsInitialiser::
initialise(const llvm::GlobalVariable* llvm_var) {
  const auto llvm_init = llvm_var->getInitializer();
  auto id = globals.request(
    llvm_var, sizes.getSizeOf(llvm_init->getType()) + 1);
  (void) globals.emplaceGlobalReference(id, id + 1);
  // `id + 1` is the first MemoryAtom used for data:
  return initialise(llvm_init, id + 1);
}

bool
racehog::GlobalsInitialiser::
initialise(const llvm::Constant* llvm_var,
           MemoryAtomId          id) {
  // Ordered roughly by expected ratio of occurance.

  // TODO:  Work on this list:
  //
  // [ ] Constant:
  //     [ ] BlockAddress
  //     [ ] ConstantAggregate:
  //         [x] ConstantArray
  //         [x] ConstantStruct
  //         [ ] ConstantVector
  //     [ ] ConstantData:
  //         [x] ConstantAggregateZero
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
  if (llvm::isa<llvm::ConstantAggregateZero>(llvm_var) == true ||
      llvm::isa<llvm::ConstantFP>(llvm_var)            == true ||
      llvm::isa<llvm::ConstantInt>(llvm_var)           == true ||
      llvm::isa<llvm::ConstantPointerNull>(llvm_var)   == true)
    return true; // Nothing to do;  bail out.
  if (const auto llvm_arr = llvm::dyn_cast<llvm::ConstantArray>(llvm_var)) {
    const auto atype     = llvm_arr->getType();
    auto       elem_size = sizes.getSizeOf(atype->getElementType());
    const auto num_elems = atype->getNumElements();
    for (unsigned i = 0; i < num_elems; ++i) {
      if (initialise(llvm_arr->getAggregateElement(i), id) == false)
        return false;
      id += elem_size;
    }
    return true;
  }
  if (const auto llvm_s = llvm::dyn_cast<llvm::ConstantStruct>(llvm_var)) {
    const auto stype = llvm_s->getType();
    const auto num_elems = stype->getNumElements();
    for (unsigned i = 0; i < num_elems; ++i) {
      if (initialise(llvm_s->getAggregateElement(i), id) == false)
        return false;
      id += sizes.getSizeOf(stype->getElementType(i));
    }
    return true;
  }
  if (const auto llvm_func = llvm::dyn_cast<llvm::Function>(llvm_var)) {
    auto& eq = funcs.getEntryPoint()->append<EqualiseInstruction>();
    (void) eq.append<FunctionOperand>(funcs.get(llvm_func));
    (void) eq.append<GlobalMemoryAtomOperand>(id);
    return true;
  }
  if (const auto llvm_expr = llvm::dyn_cast<llvm::ConstantExpr>(llvm_var)) {
    // TODO:  Deal with all other types as well.
    switch (llvm_expr->getOpcode()) {
    case llvm::Instruction::BitCast:
      // Simply forward the inner type:
      return initialise(llvm_expr->getOperand(0), id);
    default:
      llvm::errs() << "Unexpected constant expression: " << *llvm_expr << "\n";
      return false;
    }
  }
  if (const auto llvm_gvar = llvm::dyn_cast<llvm::GlobalVariable>(llvm_var)) {
    if (globals.contains(llvm_gvar) == false)
      initialise(llvm_gvar);
    // TODO:  Consider using ReferenceInstruction instead of EqualiseInstruction
    // to avoid unifying deep data structures.
    auto& eq = funcs.getEntryPoint()->append<EqualiseInstruction>();
    (void) eq.append<GlobalMemoryAtomOperand>(id);
    (void) eq.append<GlobalMemoryAtomOperand>(globals.lookup(llvm_gvar));
    return true;
  }
  llvm::errs() << "Unable to initialise: " << *llvm_var << "\n";
  return false;
}
