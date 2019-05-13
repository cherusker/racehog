#ifndef STEENX_LLVM_MEMORY_MEMORYALLOCATOR_H_
#define STEENX_LLVM_MEMORY_MEMORYALLOCATOR_H_

#include <unordered_map>

#include <boost/core/noncopyable.hpp>

#include "llvm/IR/Value.h"

#include "Support/Stream/Stream.h"

namespace racehog {

  /// Maps `llvm::Value*` to MemoryAtomId.

  class MemoryAllocator final
    : private boost::noncopyable {

  public:
    
    typedef Stream::MemoryAtomId MemoryAtomId;

    MemoryAtomId
    getNumAtoms(void) const {
      return num_alloc_atoms;
    }

    bool
    contains(const llvm::Value* llvm_val) const;

    /// Only safe to use if MemoryAllocator::contains is `true`.

    MemoryAtomId
    lookup(const llvm::Value* llvm_val) const;

    /// Request a new and unique memory location.
    /**
     * No `llvm::Value` will ever be accociated with the return value of
     * MemoryAllocator::request and every call to MemoryAllocator::request
     * yields a new and unique id.
     */

    MemoryAtomId
    request(MemoryAtomId num_atoms = 1);

    /// Request the id of a specific value.
    /**
     * \param llvm_val Will be inserted with a new id if it is not yet part of
     *        this MemoryAllocator.
     *        Acts like MemoryAllocator::request() if \p llvm_val is `nullptr`.
     * \param num_atoms Will be used if \p llvm_val is inserted.
     *        If \p llvm_val exists already, \p num_atoms will be ignored.
     */

    MemoryAtomId
    request(const llvm::Value* llvm_val,
            MemoryAtomId       num_atoms = 1);

    /// Erases all stored elements.

    void
    reset(void);

  private:

    typedef std::unordered_map<
      const llvm::Value*,
      MemoryAtomId> Map;

    MemoryAtomId num_alloc_atoms = 0;
    Map          map;
  };
}

#endif // STEENX_LLVM_MEMORY_MEMORYALLOCATOR_H_
