#ifndef STEENX_LLVM_CALLHANDLER_CALLHANDLERREGISTRY_H_
#define STEENX_LLVM_CALLHANDLER_CALLHANDLERREGISTRY_H_

#include <memory>
#include <unordered_map>

#include <boost/core/noncopyable.hpp>

#include "llvm/IR/Instruction.h"

#include "SteenX/LLVM/CallHandler/CallHandler.h"
#include "SteenX/LLVM/CallHandler/CallHandlerToolbox.h"
#include "SteenX/LLVM/CallHandler/RegularCallHandler.h"
#include "SteenX/LLVM/OperandTranslator.h"

namespace racehog {

  /// Registry that holds all available CallHandler objects.

  class CallHandlerRegistry final
    : private boost::noncopyable {

  public:

    CallHandlerRegistry(OperandTranslator& optrans,
                        MemoryManager&     locals,
                        MetadataExtractor& metax);

    /// Lookup a suitable CallHandler for a specific `llvm::CallInst`.
    /**
     * Returns `nullptr` no suitable CallHandler can be found.
     *
     * Tries to find a specific CallHandler first.
     * If that fails, RegularCallHandler will be used.
     */

    bool
    resolve(Function&             func,
            const llvm::CallInst& llvm_inst);

  private:

    /// Lookup per hashed name should ensure fast resolution times.
    /**
     * Boost's `ptr_unordered_map` requires RTTI (what we turn off) so
     * `std::unique_ptr` is the next best thing.
     */

    // We expect function names to be unique (no overloading like in C++).
    //   We have to update the whole process a bit, should that change one day:
    //   CallHandlerRegistry has to be installed with full function signatures
    //   and _should_ we stay with the main lookup being the name, we need a
    //   MULTImap.

    /// There is no need to store CallHandler next to each other.
    /**
     * CallHandler objects are not used very frequently and using a specific
     * CallHandler not imply using another one.
     * A map of pointers does fine here.
     */

    typedef std::unordered_map<
      std::string,
      std::unique_ptr<CallHandler>> HandlersContainer;

    CallHandlerToolbox tools;
    RegularCallHandler fallback_handler;
    HandlersContainer  handlers;

    /// "Empty" call handler;  basically a `nop`.
    /**
     * More than anything, registering a function signature through this version
     * of CallHandlerRegistry::reg ensures that RegularCallHandler does not
     * consume it.
     *
     * Fun fact:  this function is not called "register" because "register" is
     * an _unused_ keyword in C++ ...
     */

    void
    reg(const std::string& name) {
      (void) handlers.insert(std::make_pair(name, nullptr));
    }

    template<class CallHandlerType>
    void
    reg(const std::string& name) {
      (void) handlers.insert(
        std::make_pair(name, std::make_unique<CallHandlerType>(tools)));
    }
  };
}

#endif // STEENX_LLVM_CALLHANDLER_CALLHANDLERREGISTRY_H_
