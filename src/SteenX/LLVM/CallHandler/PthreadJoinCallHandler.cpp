#include "SteenX/LLVM/CallHandler/PthreadJoinCallHandler.h"

#include "SteenX/Program/Instruction/EqualiseInstruction.h"
#include "SteenX/Program/Instruction/JoinInstruction.h"
#include "SteenX/Program/Operand/LocalMemoryAtomOperand.h"
#include "Support/Logging.h"

bool
racehog::PthreadJoinCallHandler::
apply(Function&             func,
      const llvm::CallInst& llvm_inst) {
  // The return value of `phtread_join' is just `int' and can be ignored.
  // As for the arguments:
  //   1. pthread_t: thread.
  //   2. void**:    return value.
  const auto meta = metax.extractFrom(llvm_inst);
  auto thread     = optrans.preload(func, meta, llvm_inst.getArgOperand(0));
  auto& join      = func.append<JoinInstruction>(meta);
  (void) join.append<LocalMemoryAtomOperand>(locals.request());
  if (thread.appendAsValueTo(join) == false) {
    LOG(ERROR) << "Translation::appendAsValueTo failed.";
    return false;
  }
  auto retval = optrans.preload(func, meta, llvm_inst.getArgOperand(1));
  // It is somewhat likely that `retval` is `nullptr`;
  //   (`llvm::ConstantPointerNull`);   no need to equalise that :)
  if (retval.isConstantOperand() == true)
    return true;
  auto& eq = func.append<EqualiseInstruction>(meta);
  (void) eq.append(join.getReturnValue());
  if (retval.appendAsPointerTo(eq) == false) {
    LOG(ERROR) << "Translation::appendAsPointerTo failed.";
    return false;
  }
  return true;
}
