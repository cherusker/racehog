#include "SteenX/LLVM/CallHandler/MallocCallHandler.h"

#include "llvm/IR/Constant.h"

#include "SteenX/Program/Instruction/AllocateInstruction.h"
#include "SteenX/Program/Operand/LocalMemoryAtomOperand.h"
#include "Support/Logging.h"

bool
racehog::MallocCallHandler::
apply(Function&             func,
      const llvm::CallInst& llvm_inst) {
  const auto meta = metax.extractFrom(llvm_inst);
  auto llvm_size = llvm::dyn_cast<llvm::ConstantInt>(
    llvm_inst.getArgOperand(0));
  if (llvm_size               == nullptr ||
      llvm_size->isNegative() == true) {
    LOG(ERROR) << "Constant positive size expected.";
    return false;
  }
  auto& alloc = func.append<AllocateInstruction>(
    meta, llvm_size->getZExtValue());
  (void) alloc.append<LocalMemoryAtomOperand>(locals.request(&llvm_inst));
  return true;
}
