#include "SteenX/LLVM/FunctionTable.h"

racehog::FunctionTable::
FunctionTable(Program& prog)
  : prog(prog) {
  entry = prog.createFunction();
  (void) prog.setEntryPoint(entry);
}

racehog::FunctionReference
racehog::FunctionTable::
get(const llvm::Function* llvm_func) {
  // std::iterator<llvm::Function*, Stream::FunctionId>
  auto find = lookup.find(llvm_func);
  if (find != lookup.end())
    return prog.getFunctionReference(find->second);
  // TODO:  Verify copy elision here.
  auto func = prog.createFunction();
  (void) lookup.insert(std::make_pair(llvm_func, func.getId()));
  return func;
}
