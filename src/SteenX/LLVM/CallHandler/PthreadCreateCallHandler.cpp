#include "SteenX/LLVM/CallHandler/PthreadCreateCallHandler.h"

#include "SteenX/Program/Instruction/EqualiseInstruction.h"
#include "SteenX/Program/Instruction/ForkInstruction.h"
#include "SteenX/Program/Operand/LocalMemoryAtomOperand.h"
#include "Support/Logging.h"

bool
racehog::PthreadCreateCallHandler::
apply(Function&             func,
      const llvm::CallInst& llvm_inst) {
  // The return value of `phtread_create' is just `int' and can be ignored.
  // As for the arguments:
  //   1. pthread_t*:        thread;  needs dereferencing.
  //   2. pthread_attr_t*:   Will be ignored.
  //   3. void* (*) (void*): func;  has to be called.
  //   4. void*:             arg;  used in function call.
  const auto meta = metax.extractFrom(llvm_inst);
  auto fnop       = optrans.preload(func, meta, llvm_inst.getArgOperand(2));
  auto arg        = optrans.preload(func, meta, llvm_inst.getArgOperand(3));
  auto& fork      = func.append<ForkInstruction>(meta);
  if (fnop.appendAsValueTo(fork) == false) {
    LOG(ERROR) << "Translation::appendTo failed.";
    return false;
  }
  (void) fork.append<LocalMemoryAtomOperand>(locals.request());
  if (arg.appendAsValueTo(fork) == false) {
    LOG(ERROR) << "Translation::appendAsValueTo failed.";
    return false;
  }
  auto thread = optrans.preload(func, meta, llvm_inst.getArgOperand(0));
  auto& eq    = func.append<EqualiseInstruction>(meta);
  (void) eq.append(fork.getThreadPointer());
  if (thread.appendAsPointerTo(eq) == false) {
    LOG(ERROR) << "Translation::appendAsPointerTo failed.";
    return false;
  }
  return true;
}
