#include "SteenX/LLVM/SteenXTranslationPass.h"

#include <iostream>

#include "llvm/IR/DebugInfoMetadata.h"

#include "SteenX/LLVM/InstructionExtractor.h"
#include "SteenX/Program/Bitfile.h"
#include "SteenX/Program/Function.h"

// TODO:  Revisit the pass setup + check if that's (still) the way to do it.

char racehog::SteenXTranslationPass::ID = 0;

static llvm::RegisterPass<racehog::SteenXTranslationPass> X(
  "steenx-translation", // Command line argument.
  "SteenX Translation", // Name.
  false,                // Only looks at CFG.
  false);               // Analysis pass.

llvm::cl::opt<std::string> racehog::outputFilename(
  "xirfile",
  llvm::cl::desc("Specify the SteenX IR output file"),
  llvm::cl::value_desc("filename"));

racehog::SteenXTranslationPass::
~SteenXTranslationPass(void) {
#if 0
  UserStream stream(std::cerr);
  (void) prog.print(stream);
  stream << "\n";
#endif // 0
  // Call `main` function at the very end of the entry point function:
  if (llvm_entry == nullptr)
    (void) llvm::report_fatal_error("Missing entry point.", false);
  auto entry     = funcs.getEntryPoint();
  auto num_atoms = entry->getNumLocalMemoryAtoms();
  (void) entry->setNumLocalMemoryAtoms(num_atoms + 1);
  auto& call = entry->append<CallInstruction>();
  (void) call.append<FunctionOperand>(funcs.get(llvm_entry));
  (void) call.append<LocalMemoryAtomOperand>(num_atoms);
  // Store num global memory atoms:
  (void) prog.setNumGlobalMemoryAtoms(globals.getNumAtoms());
  // Write program into file:
  Bitfile file(outputFilename);
  if (file.write(prog) == false)
    (void) llvm::report_fatal_error("Unable to write to given file.", false);
}

bool
racehog::SteenXTranslationPass::
runOnFunction(llvm::Function& llvm_func) {
  if (llvm_func.getName().equals("main") == true)
    llvm_entry = &llvm_func;
  auto func = funcs.get(&llvm_func);
  (void) func->setMetadata(metax.extractFrom(llvm_func));
  (void) locals.resetAllocator();
  (void) locals.setFunction(func.getRaw());
  for (const auto& arg : llvm_func.args())
    (void) locals.request(&arg);
  // TODO:  Have InstructionExtractor as class member (do not destroy + rebuild
  // it every time -- that's unncessary overhead ...)
  InstructionExtractor ex(
    *func,
    sizes,
    atomics,
    optrans,
    locals,
    call_handlers,
    metax);
  (void) ex.visit(llvm_func);
  (void) func->setNumLocalMemoryAtoms(locals.getNumAtoms());
  return false;
}
