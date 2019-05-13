#ifndef STEENX_LLVM_CALLHANDLER_PTHREADCREATECALLHANDLER_H_
#define STEENX_LLVM_CALLHANDLER_PTHREADCREATECALLHANDLER_H_

#include "SteenX/LLVM/CallHandler/CallHandler.h"
#include "SteenX/LLVM/CallHandler/CallHandlerToolbox.h"
#include "SteenX/LLVM/Memory/MemoryManager.h"
#include "SteenX/LLVM/MetadataExtractor.h"
#include "SteenX/LLVM/OperandTranslator.h"

namespace racehog {

  /// `int pthread_create(...)`.

  class PthreadCreateCallHandler final
    : public CallHandler {

  public:

    PthreadCreateCallHandler(CallHandlerToolbox& tools)
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

#endif // STEENX_LLVM_CALLHANDLER_PTHREADCREATECALLHANDLER_H_
