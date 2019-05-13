#ifndef STEENX_LLVM_CALLHANDLER_CALLHANDLERTOOLBOX_H_
#define STEENX_LLVM_CALLHANDLER_CALLHANDLERTOOLBOX_H_

#include <boost/core/noncopyable.hpp>

#include "SteenX/LLVM/Memory/MemoryManager.h"
#include "SteenX/LLVM/MetadataExtractor.h"
#include "SteenX/LLVM/OperandTranslator.h"

namespace racehog {

  /// Helps with dependency injection in CallHandler objects.

  class CallHandlerToolbox final
    : private boost::noncopyable {

  public:

    CallHandlerToolbox(MemoryManager&     locals,
                       OperandTranslator& optrans,
                       MetadataExtractor& metax)
      : locals(locals),
        optrans(optrans),
        metax(metax) {}

    OperandTranslator&
    getOperandTranslator(void) {
      return optrans;
    }

    MemoryManager&
    getLocalMemoryManager(void) {
      return locals;
    }

    MetadataExtractor&
    getMetadataExtractor(void) {
      return metax;
    }

  private:

    MemoryManager&     locals;
    OperandTranslator& optrans;
    MetadataExtractor& metax;
  };
}

#endif // STEENX_LLVM_CALLHANDLER_CALLHANDLERTOOLBOX_H_
