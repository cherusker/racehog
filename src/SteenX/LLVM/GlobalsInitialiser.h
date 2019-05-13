#ifndef STEENX_LLVM_GLOBALSINITIALISER_H_
#define STEENX_LLVM_GLOBALSINITIALISER_H_

#include <boost/core/noncopyable.hpp>

#include "llvm/IR/GlobalVariable.h"

#include "SteenX/LLVM/FunctionTable.h"
#include "SteenX/LLVM/TypeSizesTable.h"
#include "SteenX/LLVM/Memory/MemoryManager.h"

namespace racehog {

  /// Initialiser for `llvm::GlobalVariable`.

  class GlobalsInitialiser final
    : private boost::noncopyable {

  public:

    typedef MemoryManager::MemoryAtomId MemoryAtomId;

    GlobalsInitialiser(TypeSizesTable& sizes,
                       FunctionTable&  funcs,
                       MemoryManager&  globals)
      : sizes(sizes),
        funcs(funcs),
        globals(globals) {}

    /// Return `true` if initialisation was successful.

    bool
    initialise(const llvm::GlobalVariable* llvm_var);

  private:

    TypeSizesTable& sizes;
    FunctionTable&  funcs;
    MemoryManager&  globals;

    bool
    initialise(const llvm::Constant* llvm_var,
               MemoryAtomId          id);
  };
}

#endif // STEENX_LLVM_GLOBALSINITIALISER_H_
