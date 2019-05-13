#ifndef STEENX_LLVM_MEMORY_MEMORYMANAGER_H_
#define STEENX_LLVM_MEMORY_MEMORYMANAGER_H_

#include <boost/core/noncopyable.hpp>

#include "llvm/IR/Value.h"

#include "SteenX/LLVM/Memory/MemoryAllocator.h"
#include "SteenX/Program/Function.h"
#include "SteenX/Program/Instruction/Instruction.h"
#include "Support/Stream/Stream.h"

namespace racehog {

  /// Manages a sub category of the memory (`global` or `local`).
  /**
   * Uses a MemoryAllocator internally to keep track of mappings from
   * `llvm::Value*` to MemoryAtomId.
   * On top of that, MemoryManager adds a few convenience functions like
   * allocating several atoms at once.
   */

  class MemoryManager final
    : private boost::noncopyable {

  public:

    typedef Stream::MemoryAtomId MemoryAtomId;

    MemoryManager(void) = default;

    MemoryManager(Function* func) {
      (void) setFunction(func);
    }

    /// Function that is used in MemoryManager::emplaceLocalReference et al.

    void
    setFunction(Function* func) {
      this->func = func;
    }

    MemoryAtomId
    getNumAtoms(void) const {
      return alloc.getNumAtoms();
    }

    bool
    contains(const llvm::Value* llvm_val) const {
      return alloc.contains(llvm_val);
    }

    MemoryAtomId
    lookup(const llvm::Value* llvm_val) const {
      return alloc.lookup(llvm_val);
    }

    MemoryAtomId
    request(MemoryAtomId num_atoms = 1) {
      return alloc.request(num_atoms);
    }

    MemoryAtomId
    request(const llvm::Value* llvm_val,
            MemoryAtomId       num_atoms = 1) {
      return alloc.request(llvm_val, num_atoms);
    }

    /// Emplace ReferenceInstruction:  `ptr -> val`.
    /**
     * Uses two LocalMemoryAtomOperand objects.
     */

    void
    emplaceLocalReference(const Instruction::MetadataReference& meta,
                          MemoryAtomId                          ptr,
                          MemoryAtomId                          val);

    /// Similar to MemoryManagere::emplaceLocalReference.

    void
    emplaceGlobalReference(MemoryAtomId ptr,
                           MemoryAtomId val);

    void
    resetAllocator(void) {
      (void) alloc.reset();
    }

  private:

    MemoryAllocator alloc;
    Function*       func = nullptr;
  };
}

#endif // STEENX_LLVM_MEMORY_MEMORYMANAGER_H_
