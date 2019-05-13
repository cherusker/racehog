#include "SteenX/LLVM/AtomicLocksTable.h"

racehog::AtomicLocksTable::MemoryAtomId
racehog::AtomicLocksTable::
getLockMemoryAtomId(const llvm::Value* llvm_val) {
  const auto search = ids.find(llvm_val);
  if (search != ids.end())
    return search->second;
  auto  id = globals.request(2);
  // No Metadata available.
  (void) globals.emplaceGlobalReference(id, id + 1);
  (void) ids.insert(std::make_pair(llvm_val, id));
  return id;
}
