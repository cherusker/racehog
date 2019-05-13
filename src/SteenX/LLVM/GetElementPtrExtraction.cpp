#include "SteenX/LLVM/GetElementPtrExtraction.h"

#include "Support/Macros.h"

#include "llvm/IR/Constants.h"

void
racehog::GetElementPtrExtraction::
setup(const llvm::User* gep) {
  llvm_it  = gep->op_begin();
  llvm_end = gep->op_end();
  // The first operand holds the base:
  llvm_type = (llvm_it++)->get()->getType();
  (void) advance();
}

bool
racehog::GetElementPtrExtraction::
advance(void) {
  base   = 0;
  repeat = 0;
  spread = 0;
  if (llvm_it == llvm_end)
    return false;
  while (llvm_it != llvm_end) {
    // Post increment isn't too bad here;  `typeof(llvm_it) == Use*`.
    // Increment upfront as the loop could be terminated by `return`.
    const auto llvm_idx = (llvm_it++)->get();
    if (auto llvm_cidx = llvm::dyn_cast<llvm::ConstantInt>(llvm_idx)) {
      auto idx = llvm_cidx->getSExtValue();
      if (const auto p = llvm::dyn_cast<llvm::PointerType>(llvm_type)) {
        llvm_type = p->getElementType();
        base      = idx * sizes.getSizeOf(llvm_type);
      } else if (const auto a = llvm::dyn_cast<llvm::ArrayType>(llvm_type)) {
        llvm_type = a->getElementType();
        base      = idx * sizes.getSizeOf(llvm_type);
      } else if (const auto s = llvm::dyn_cast<llvm::StructType>(llvm_type)) {
        llvm_type = s->getElementType(idx);
        for (unsigned i = 0; i < idx; ++i)
          base += sizes.getSizeOf(s->getElementType(i));
      } else {
        LLVM_FATAL_EXIT(
          *llvm_type, "Unexpected base type with static indexing.");
      }
    } else {
      // TODO:  Make available for raw pointers one day.
      if (const auto a = llvm::dyn_cast<llvm::ArrayType>(llvm_type)) {
        llvm_type = a->getElementType();
        repeat    = a->getNumElements() - 1; // 2 elements -> 1 repetition.
        spread    = sizes.getSizeOf(llvm_type);
        return true; // Full stop after range accesses.
      }
      LLVM_FATAL_EXIT(
        *llvm_type, "Unexpected base type with dynamic indexing.");
    }
  }
  return true;
}
