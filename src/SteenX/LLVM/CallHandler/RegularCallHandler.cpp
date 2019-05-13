#include "SteenX/LLVM/CallHandler/RegularCallHandler.h"

#include "SteenX/Program/Instruction/CallInstruction.h"
#include "SteenX/Program/Operand/LocalMemoryAtomOperand.h"
#include "Support/Logging.h"

bool
racehog::RegularCallHandler::
apply(Function&             func,
      const llvm::CallInst& llvm_inst) {
  const auto meta = metax.extractFrom(llvm_inst);
  auto fnop       = optrans.preload(func, meta, llvm_inst.getCalledValue());
  auto& call      = func.append<CallInstruction>(meta);
  if (fnop.appendAsValueTo(call) == false) {
    LOG(ERROR) << "Translation::appendAsValueTo failed.";
    return false;
  }
  (void) call.append<LocalMemoryAtomOperand>(locals.request(&llvm_inst));
  for (const auto& llvm_arg : llvm_inst.arg_operands()) {
    // TODO:  This needs fixing.  Preload looses all meaning here.   Possible
    // (though) unclean:  preload before creating CallInstruction and check
    // WITHIN preload if stuff is already a local variable in which case skip
    // the preload ...
    auto arg = optrans.preload(func, meta, llvm_arg);
    if (arg.appendAsValueTo(call) == false) {
      LOG(ERROR) << "Translation::appendAsValueTo failed.";
      return false;
    }
  }
  return true;
}
