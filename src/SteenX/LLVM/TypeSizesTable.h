#ifndef STEENX_LLVM_TYPESIZESTABLE_H_
#define STEENX_LLVM_TYPESIZESTABLE_H_

#include <unordered_map>

#include <boost/core/noncopyable.hpp>

#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Type.h"

#include "Support/Stream/Stream.h"

namespace racehog {

  /// Size lookup for arbitrary LLVM types.
  /**
   * Special trick of TypeSizesTable is an internal buffer to store the size of
   * aggregate types on all levels and avoid repetetive recursive decents.
   */

  class TypeSizesTable final
    : private boost::noncopyable {

  public:

    typedef Stream::MemoryAtomId TypeSize;

    /// The amount of required memory atoms to store \p type in memory.
    /**
     * \param type Cannot be `nullptr`.
     */

    TypeSize
    getSizeOf(const llvm::Type* llvm_type);

    /// The offset between the start of an aggregate and the indexed field.

    TypeSize
    getOffsetOf(const llvm::Type*        llvm_type,
                llvm::ArrayRef<unsigned> indices);

  private:

    /// Buffer storage of aggregate types.

    typedef std::unordered_map<const llvm::Type*, TypeSize> TypeSizes;

    TypeSizes sizes;
  };
}

#endif // STEENX_LLVM_TYPESIZESTABLE_H_
