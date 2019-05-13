#ifndef STEENX_LLVM_CALLHANDLER_PTHREADMUTEXLOCKCALLHANDLER_H_
#define STEENX_LLVM_CALLHANDLER_PTHREADMUTEXLOCKCALLHANDLER_H_

#include "SteenX/LLVM/CallHandler/CallHandler.h"
#include "SteenX/LLVM/CallHandler/CallHandlerToolbox.h"
#include "SteenX/LLVM/Memory/MemoryManager.h"
#include "SteenX/LLVM/MetadataExtractor.h"
#include "SteenX/LLVM/OperandTranslator.h"

namespace racehog {

  /// `int pthread_mutex_*lock(pthread_mutex_t*, ...)`.
  /**
   * Can be used for all sorts of lock functions with the same function
   * signature;  like `pthread_mutex_trylock` or `pthread_mutex_timedlock`.
   */

  class PthreadMutexLockCallHandler final
    : public CallHandler {

  public:

    PthreadMutexLockCallHandler(CallHandlerToolbox& tools)
      : locals(tools.getLocalMemoryManager()),
        optrans(tools.getOperandTranslator()),
        metax(tools.getMetadataExtractor()) {}

    bool
    apply(Function&             func,
          const llvm::CallInst& llvm_inst) override;

  private:

    MemoryManager&     locals;
    OperandTranslator& optrans;
    MetadataExtractor& metax;
  };
}

#endif // STEENX_LLVM_CALLHANDLER_PTHREADMUTEXLOCKCALLHANDLER_H_
