#ifndef STEENX_LLVM_CALLHANDLER_CALLHANDLER_H_
#define STEENX_LLVM_CALLHANDLER_CALLHANDLER_H_

#include <boost/core/noncopyable.hpp>

#include "llvm/IR/Instructions.h"

#include "SteenX/Program/Function.h"

namespace racehog {

  /// Base class of all call handlers.

  class CallHandler
    : private boost::noncopyable {

  public:

    virtual
    ~CallHandler(void) = default;

    /// Apply `llvm::CallInst` to Function.
    /**
     * Deriving CallHandler objects can expect to be called iff the function
     * signature within \p llvm_inst matches exactly what that CallHandler
     * objects expect.
     */

    virtual bool
    apply(Function&             func,
          const llvm::CallInst& llvm_inst) = 0;
  };
}

#endif // STEENX_LLVM_CALLHANDLER_CALLHANDLER_H_
