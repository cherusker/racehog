#ifndef STEENX_LLVM_FUNCTIONTABLE_H_
#define STEENX_LLVM_FUNCTIONTABLE_H_

#include <unordered_map>

#include <boost/core/noncopyable.hpp>

#include "llvm/IR/Function.h"
#include "llvm/IR/Value.h"

#include "SteenX/Program/FunctionReference.h"
#include "SteenX/Program/Program.h"

namespace racehog {

  /// A symbol table to translate `llvm::Function` to FunctionReference.

  class FunctionTable final
    : private boost::noncopyable {

  public:

    FunctionTable(Program& prog);

    /// Get a FunctionReference to a (unqiue) entry point Function.
    /**
     * Every call to FunctionTable::getEntryPoint will yield a
     * FunctionReference to the _same_ Function.
     */

    FunctionReference
    getEntryPoint(void) const {
      return prog.getEntryPoint();
    }

    /// Translate `llvm::Function*` to FunctionReference.
    /**
     * If FunctionTable::get sees some `llvm::Function*` for the first time, it
     * will associate a unique (new) Function with it.
     */

    FunctionReference
    get(const llvm::Function* llvm_func);

  private:

    typedef std::unordered_map<
      const llvm::Function*,
      Function::Id> LookupContainer;

    Program&        prog;
    LookupContainer lookup;

    FunctionReference entry;
  };
}

#endif // STEENX_LLVM_FUNCTIONTABLE_H_
