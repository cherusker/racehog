#include "SteenX/LLVM/TypeSizesTable.h"

#include "llvm/Support/raw_ostream.h"

#include "Support/Macros.h"

racehog::TypeSizesTable::TypeSize
racehog::TypeSizesTable::
getSizeOf(const llvm::Type* llvm_type) {
  // TODO:  Do these types cover all (interesting) elementary types?
  if (llvm_type->isPointerTy()       == true ||
      llvm_type->isIntegerTy()       == true ||
      llvm_type->isFloatingPointTy() == true)
    return 1; // Occupying exactly one memory atom.
  const auto match = sizes.find(llvm_type);
  if (match != sizes.end())
    return match->second;
  // TODO:  Convert recursion into iterative loop.
  TypeSize size = 0;
  if (const auto a = llvm::dyn_cast<llvm::ArrayType>(llvm_type)) {
    size += getSizeOf(a->getElementType()) * a->getNumElements();
  } else if (const auto s = llvm::dyn_cast<llvm::StructType>(llvm_type)) {
    for (const auto sub : s->elements())
      size += getSizeOf(sub);
  } else {
    // TODO:  deal with other subtypes?
    //   - VectorType
    //   - FunctionType
    LLVM_FATAL_EXIT(*llvm_type, "Unexpected type.");
  }
  (void) sizes.insert(std::make_pair(llvm_type, size));
  return size;
}

racehog::TypeSizesTable::TypeSize
racehog::TypeSizesTable::
getOffsetOf(const llvm::Type*        llvm_type,
            llvm::ArrayRef<unsigned> indices) {
  TypeSize offset = 0;
  for (auto i : indices) {
    if (const auto p = llvm::dyn_cast<llvm::PointerType>(llvm_type)) {
      llvm_type = p->getElementType();
      offset += i * getSizeOf(llvm_type);
    } else if (const auto a = llvm::dyn_cast<llvm::ArrayType>(llvm_type)) {
      llvm_type = a->getElementType();
      offset += i * getSizeOf(llvm_type);
    } else if (const auto s = llvm::dyn_cast<llvm::StructType>(llvm_type)) {
      llvm_type = s->getElementType(i);
      for (unsigned si = 0; si < i; ++si)
        offset += getSizeOf(s->getElementType(si));
    } else {
      LLVM_FATAL_EXIT(*llvm_type, "Unexpected type for indexing.");
    }
  }
  return offset;
}
