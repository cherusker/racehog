#ifndef STEENX_LLVM_CALLHANDLER_MALLOCCALLHANDLER_H_
#define STEENX_LLVM_CALLHANDLER_MALLOCCALLHANDLER_H_

#include "SteenX/LLVM/CallHandler/CallHandler.h"
#include "SteenX/LLVM/CallHandler/CallHandlerToolbox.h"
#include "SteenX/LLVM/Memory/MemoryManager.h"
#include "SteenX/LLVM/MetadataExtractor.h"

namespace racehog {

  /// `void* malloc(size_t size)`.

  class MallocCallHandler final
    : public CallHandler {

  public:

    MallocCallHandler(CallHandlerToolbox& tools)
      : locals(tools.getLocalMemoryManager()),
        metax(tools.getMetadataExtractor()) {}

    /// Memory for `malloc` is currently overestimated to guarantee correctness.
    /**
     * In its current form, MallocCallHandler requests as many memory atoms as
     * malloc requests bytes.
     * This heavily overestimates (4 - 8 times on average) the actual number of
     * memory atoms needed, unless `malloc` was requested for `char`.
     */

    bool
    apply(Function&             func,
          const llvm::CallInst& llvm_inst) override;

  private:

    MemoryManager&     locals;
    MetadataExtractor& metax;
  };
}

#endif // STEENX_LLVM_CALLHANDLER_MALLOCCALLHANDLER_H_
