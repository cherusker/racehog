#ifndef STEENX_LLVM_ATOMICLOCKSTABLE_H_
#define STEENX_LLVM_ATOMICLOCKSTABLE_H_

#include <unordered_map>

#include <boost/core/noncopyable.hpp>

#include "llvm/IR/Value.h"

#include "SteenX/LLVM/Memory/MemoryManager.h"
#include "Support/Stream/Stream.h"

namespace racehog {

  /// Translates `llvm::Value` to Stream::MemoryAtomId.

  class AtomicLocksTable final
    : private boost::noncopyable {

  public:

    typedef Stream::MemoryAtomId MemoryAtomId;

    AtomicLocksTable(MemoryManager& globals)
      : globals(globals) {}

    MemoryAtomId
    getLockMemoryAtomId(const llvm::Value* llvm_val);

  private:

    typedef std::unordered_map<
      const llvm::Value*,
      MemoryAtomId> MemoryAtomIds;

    MemoryManager& globals;
    MemoryAtomIds  ids;
  };
}

#endif // STEENX_LLVM_ATOMICLOCKSTABLE_H_
