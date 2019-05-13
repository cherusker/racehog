#ifndef STEENX_LLVM_STEENXTRANSLATIONPASS_H_
#define STEENX_LLVM_STEENXTRANSLATIONPASS_H_

#include <boost/core/noncopyable.hpp>

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"

#include "SteenX/LLVM/AtomicLocksTable.h"
#include "SteenX/LLVM/CallHandler/CallHandlerRegistry.h"
#include "SteenX/LLVM/FunctionTable.h"
#include "SteenX/LLVM/Memory/MemoryManager.h"
#include "SteenX/LLVM/MetadataExtractor.h"
#include "SteenX/LLVM/OperandTranslator.h"
#include "SteenX/LLVM/StringTable.h"
#include "SteenX/LLVM/TypeSizesTable.h"
#include "SteenX/Program/Program.h"
#include "Support/Logging.h"

namespace racehog {

  extern llvm::cl::opt<std::string> outputFilename;

  class SteenXTranslationPass final
    : public  llvm::FunctionPass,
      private boost::noncopyable {

  public:

    static char ID; ///< LLVM requirement.

    SteenXTranslationPass(void)
      : FunctionPass(ID),
        strings(prog.getStringPool()),
        funcs(prog),
        globals(funcs.getEntryPoint().getRaw()),
        ginit(sizes, funcs, globals),
        atomics(globals),
        optrans(sizes, funcs, globals, locals, ginit),
        metax(strings, prog.getMetadataPool()),
        call_handlers(optrans, locals, metax),
        metapool(prog.getMetadataPool().getNamedMetadata()) {
      (void) initLogging("translation-pass");
    }

    ~SteenXTranslationPass(void);

    bool
    runOnFunction(llvm::Function& func) override;

  private:

    TypeSizesTable                  sizes;
    Program                         prog;
    StringTable                     strings;
    FunctionTable                   funcs;
    MemoryManager                   globals;
    MemoryManager                   locals;
    GlobalsInitialiser              ginit;
    AtomicLocksTable                atomics;
    OperandTranslator               optrans;
    MetadataExtractor               metax;
    CallHandlerRegistry             call_handlers;
    MetadataSubpool<NamedMetadata>& metapool;

    llvm::Function* llvm_entry = nullptr;
  };
}

#endif // STEENX_LLVM_STEENXTRANSLATIONPASS_H_
