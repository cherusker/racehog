#ifndef STEENX_LLVM_GETELEMENTPTREXTRACTION_H_
#define STEENX_LLVM_GETELEMENTPTREXTRACTION_H_

#include <boost/core/noncopyable.hpp>

#include "llvm/IR/Instructions.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/User.h"

#include "SteenX/LLVM/TypeSizesTable.h"
#include "Support/Stream/Stream.h"

namespace racehog {

  /// Extraction site that iterators over indices of LLVM's `getelementptr`.
  /**
   * The general idea behind GetElementPtrExtractor is a clever, step-by-step,
   * unrolling of indices.
   * While unrolling, the (deep) size of each indexed type will be taken into
   * accout.
   * Information is provided via TypeSizesTable.
   *
   * The unrolling process pauses whenever it hits a ranged index.
   * Ranged indices depend on variables and are not constant.
   * Currently, only arrays can be indexed in range manner.
   * The calling/using class of GetElementPtrExtraction can then use all
   * information available up to that point
   * (GetElementPtrExtraction::getBaseOffset,
   * GetElementPtrExtraction::getRangeRepeat,
   * GetElementPtrExtraction::getRangeSpread) and process it.
   * After processing, the unrolling can be continued via
   * GetElementPtrExtraction::advance.
   *
   * Note that, for convenience, the first bit of unrolling is done
   * automatically.
   * A general usage pattern would, therefore, look like this:
   * `do { ... } while (extraction.advance() == true);`.
   */

  class GetElementPtrExtraction final
    : private boost::noncopyable {

  public:

    typedef Stream::MemoryAtomId            MemoryAtomId;
    typedef Stream::MemoryAtomPointerOffset PointerOffset;

    GetElementPtrExtraction(TypeSizesTable&                sizes,
                            const llvm::GetElementPtrInst& inst)
      : sizes(sizes) {
      (void) setup(&inst);
    }

    /// Obviously, \p expr must be `getelementptr`.

    GetElementPtrExtraction(TypeSizesTable&           sizes,
                            const llvm::ConstantExpr* expr)
      : sizes(sizes) {
      (void) setup(expr);
    }

    /// Static base offset since pausing unrolling last.

    PointerOffset
    getBaseOffset(void) {
      return base;
    }

    /// Current ranged index repeat.

    MemoryAtomId
    getRangeRepeat(void) {
      return repeat;
    }

    /// Current ranged index spread.

    MemoryAtomId
    getRangeSpread(void) {
      return spread;
    }

    /// `true` if GetElementPtrExtraction::advance will return `true`.

    bool
    hasMoreIndices(void) {
      return llvm_it != llvm_end;
    }
    
    /// `true` if more indices have been considered, `false` if we hit the end.

    bool
    advance(void);

  private:

    TypeSizesTable& sizes;

    // This is quite a memory footprint ...
    // Granted, `repeat` + `spread` are not needed if we can rule out ranged
    //   indices but other than that, everything is required as is, no matter if
    //   we encapsulate the logic within this class or build a dedicated system
    //   somewhere else.

    PointerOffset base   = 0;
    PointerOffset repeat = 0;
    PointerOffset spread = 0;

    llvm::User::const_op_iterator llvm_it;
    llvm::User::const_op_iterator llvm_end;

    llvm::Type* llvm_type;

    void
    setup(const llvm::User* gep);
  };
}

#endif // STEENX_LLVM_GETELEMENTPTREXTRACTION_H_
