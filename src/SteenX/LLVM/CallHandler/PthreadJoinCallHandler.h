#ifndef STEENX_LLVM_CALLHANDLER_PTHREADJOINCALLHANDLER_H_
#define STEENX_LLVM_CALLHANDLER_PTHREADJOINCALLHANDLER_H_

#include "SteenX/LLVM/CallHandler/CallHandler.h"
#include "SteenX/LLVM/CallHandler/CallHandlerToolbox.h"
#include "SteenX/LLVM/Memory/MemoryManager.h"
#include "SteenX/LLVM/MetadataExtractor.h"
#include "SteenX/LLVM/OperandTranslator.h"

namespace racehog {

  /// `int pthread_join(pthread_t, void**)`.

  class PthreadJoinCallHandler final
    : public CallHandler {

  public:

    PthreadJoinCallHandler(CallHandlerToolbox& tools)
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

#endif // STEENX_LLVM_CALLHANDLER_PTHREADJOINCALLHANDLER_H_
