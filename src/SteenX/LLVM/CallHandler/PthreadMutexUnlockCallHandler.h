#ifndef STEENX_LLVM_CALLHANDLER_PTHREADMUTEXUNLOCKCALLHANDLER_H_
#define STEENX_LLVM_CALLHANDLER_PTHREADMUTEXUNLOCKCALLHANDLER_H_

#include "SteenX/LLVM/CallHandler/CallHandler.h"
#include "SteenX/LLVM/CallHandler/CallHandlerToolbox.h"
#include "SteenX/LLVM/Memory/MemoryManager.h"
#include "SteenX/LLVM/MetadataExtractor.h"
#include "SteenX/LLVM/OperandTranslator.h"

namespace racehog {

  /// `int pthread_mutex_unlock(pthread_mutex_t*)`.

  class PthreadMutexUnlockCallHandler final
    : public CallHandler {

  public:

    PthreadMutexUnlockCallHandler(CallHandlerToolbox& tools)
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

#endif // STEENX_LLVM_CALLHANDLER_PTHREADMUTEXUNLOCKCALLHANDLER_H_
