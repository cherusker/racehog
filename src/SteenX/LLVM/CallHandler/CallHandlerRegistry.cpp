#include "SteenX/LLVM/CallHandler/CallHandlerRegistry.h"

#include "llvm/IR/Intrinsics.h"

#include "SteenX/LLVM/CallHandler/MallocCallHandler.h"
#include "SteenX/LLVM/CallHandler/PthreadCreateCallHandler.h"
#include "SteenX/LLVM/CallHandler/PthreadJoinCallHandler.h"
#include "SteenX/LLVM/CallHandler/PthreadMutexLockCallHandler.h"
#include "SteenX/LLVM/CallHandler/PthreadMutexUnlockCallHandler.h"
#include "SteenX/LLVM/MetadataExtractor.h"

racehog::CallHandlerRegistry::
CallHandlerRegistry(OperandTranslator& optrans,
                    MemoryManager&     locals,
                    MetadataExtractor& metax)
  : tools(locals, optrans, metax),
    fallback_handler(tools) {
  // Threading:
  (void) reg<PthreadCreateCallHandler>("pthread_create");
  (void) reg<PthreadJoinCallHandler>("pthread_join");
  // Mutexes:
  (void) reg("pthread_mutex_init");
  (void) reg("pthread_mutex_destroy");
  (void) reg<PthreadMutexLockCallHandler>("pthread_mutex_lock");
  (void) reg<PthreadMutexLockCallHandler>("pthread_mutex_trylock");
  (void) reg<PthreadMutexLockCallHandler>("pthread_mutex_timedlock");
  (void) reg<PthreadMutexUnlockCallHandler>("pthread_mutex_unlock");
  (void) reg("pthread_mutex_consistent");
  (void) reg("pthread_mutexattr_getrobust");
  (void) reg("pthread_mutexattr_setrobust");
  // Memory allocation:
  (void) reg<MallocCallHandler>("malloc");
  (void) reg("free");
}

bool
racehog::CallHandlerRegistry::
resolve(Function&             func,
        const llvm::CallInst& llvm_inst) {
  const auto llvm_func = llvm_inst.getCalledFunction();
  if (llvm_func != nullptr) {
    // We ignore intrinsics categorically at the moment.
    if (llvm_func->getIntrinsicID() != llvm::Intrinsic::not_intrinsic)
      return true;
    auto handler = handlers.find(llvm_func->getName());
    if (handler != handlers.end()) {
      if (handler->second == nullptr)
        return true; // Ignored functions are stored as `nullptr`.
      return handler->second->apply(func, llvm_inst);
    }
  }
  return fallback_handler.apply(func, llvm_inst);
}
