#include "SteenX/LLVM/CallHandler/PthreadMutexLockCallHandler.h"

#include "SteenX/Program/Instruction/AcquireInstruction.h"
#include "SteenX/Program/Instruction/EqualiseInstruction.h"
#include "SteenX/Program/Operand/LocalMemoryAtomOperand.h"
#include "Support/Logging.h"

bool
racehog::PthreadMutexLockCallHandler::
apply(Function&             func,
      const llvm::CallInst& llvm_inst) {
  // The return value is just `int` and can be ignored.
  const auto meta = metax.extractFrom(llvm_inst);
  auto mutex = optrans.preload(func, meta, llvm_inst.getArgOperand(0));
  auto& acq       = func.append<AcquireInstruction>(meta);
  if (mutex.appendAsPointerTo(acq) == false) {
    LOG(ERROR) << "Translation::appendAsPointerTo failed.";
    return false;
  }
  return true;
}
