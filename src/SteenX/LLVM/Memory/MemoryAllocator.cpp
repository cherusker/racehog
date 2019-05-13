#include "SteenX/LLVM/Memory/MemoryAllocator.h"

bool
racehog::MemoryAllocator::
contains(const llvm::Value* llvm_val) const {
  auto search = map.find(llvm_val);
  return search == map.end() ? false : true;
}

racehog::MemoryAllocator::MemoryAtomId
racehog::MemoryAllocator::
lookup(const llvm::Value* llvm_val) const {
  auto search = map.find(llvm_val);
  return search->second;
}

racehog::MemoryAllocator::MemoryAtomId
racehog::MemoryAllocator::
request(MemoryAtomId num_atoms) {
  auto num = num_alloc_atoms;
  num_alloc_atoms += num_atoms;
  return num;
}

racehog::MemoryAllocator::MemoryAtomId
racehog::MemoryAllocator::
request(const llvm::Value* llvm_val,
        MemoryAtomId       num_atoms) {
  if (llvm_val == nullptr)
    return request(num_atoms);
  // std::pair<iterator<llvm::Value*, Stream::MemoryAtomId>, bool>
  const auto val = map.insert(std::make_pair(llvm_val, num_alloc_atoms));
  if (val.second == true)
    return request(num_atoms);
  return val.first->second;
}

void
racehog::MemoryAllocator::
reset(void) {
  num_alloc_atoms = 0;
  (void) map.clear();
}
