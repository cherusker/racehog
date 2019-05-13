#ifndef STEENX_PROGRAM_PROGRAM_H_
#define STEENX_PROGRAM_PROGRAM_H_

#include <deque>

#include <boost/core/noncopyable.hpp>

#include "SteenX/Program/Function.h"
#include "SteenX/Program/FunctionReference.h"
#include "SteenX/Program/Metadata/MetadataPool.h"
#include "SteenX/Program/StringPool.h"
#include "Support/range_iterator.h"
#include "Support/Stream/Printable.h"
#include "Support/Stream/Serialisable.h"

namespace racehog {
  
  /// Everything that's needed to describe a program.

  class Program final
    : public  Printable,
      public  Serialisable,
      private boost::noncopyable {

    /**
     * We need a container where elements are allocated at their final resting
     * place immediately as item pointers could be taken at any time which
     * should stay valid until the end of time.
     */

    typedef std::deque<Function> Functions;

  public:

    typedef Stream::MemoryAtomId                      MemoryAtomId;
    typedef range_iterator<Functions::iterator>       FunctionsRange;
    typedef range_iterator<Functions::const_iterator> ConstFunctionsRange;

    bool
    readFromStream(InputStream& stream) override;

    bool
    writeToStream(OutputStream& stream) const override;

    StringPool&
    getStringPool(void) {
      return strings;
    }

    MetadataPool&
    getMetadataPool(void) {
      return metapool;
    }

    MemoryAtomId
    getNumGlobalMemoryAtoms(void) const {
      return num_global_atoms;
    }

    void
    setNumGlobalMemoryAtoms(MemoryAtomId num) {
      num_global_atoms = num;
    }

    FunctionReference
    createFunction(void) {
      (void) funcs.emplace_back();
      return FunctionReference(funcs.size() - 1, &funcs.back());
    }

    /// No bounds check;  know what you request!

    Function&
    getFunction(Function::Id id) {
      return funcs[id];
    }

    /// No bounds check;  know what you request!

    const Function&
    getFunction(Function::Id id) const {
      return funcs[id];
    }

    /// No bounds check;  know what you request!

    FunctionReference
    getFunctionReference(Function::Id id) {
      return FunctionReference(id, &funcs[id]);
    }

    FunctionsRange
    getFunctions(void) {
      return FunctionsRange(funcs);
    }

    ConstFunctionsRange
    getFunctions(void) const {
      return ConstFunctionsRange(funcs);
    }

    const FunctionReference&
    getEntryPoint(void) const {
      return entry;
    }

    void
    setEntryPoint(const FunctionReference& func) {
      entry = func;
    }

    void
    print(UserStream&        stream,
          const std::string& indent = "") const override;

  private:

    StringPool        strings;
    MetadataPool      metapool;
    MemoryAtomId      num_global_atoms;
    Functions         funcs;
    FunctionReference entry;
  };
}

#endif // STEENX_PROGRAM_PROGRAM_H_
