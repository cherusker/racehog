#include "SteenX/VM/Equaliser.h"

bool
racehog::Equaliser::
equalise(MemoryAtom* lhs,
         MemoryAtom* rhs) {
  if (lhs == nullptr ||
      rhs == nullptr) {
    LOG(ERROR) << "MemoryAtom is nullptr.";
    return false;
  }
  return lhs->equalise(rhs);
}

bool
racehog::Equaliser::
equalise(const Function* lhs,
         MemoryAtom*     rhs) {
  if (lhs == nullptr) {
    LOG(ERROR) << "Function is nullptr.";
    return false;
  }
  if (rhs == nullptr) {
    LOG(ERROR) << "MemoryAtom is nullptr.";
    return false;
  }
  if (rhs->getFunction() != nullptr) {
    LOG(ERROR) << "Unifying function pointers required.";
    return false;
  }
  (void) rhs->setFunction(lhs);
  return true;
}

bool
racehog::Equaliser::
equalise(MemoryAtom*    lhs,
         CallContext&   rhs_context,
         const Operand& rhs_op) {
  // Roughly ordered by expected frequency of occurence.
  if (const auto rhs_maop = rhs_op.as<MemoryAtomOperand>())
    return forwardMemoryAtomOperand(lhs, rhs_context, rhs_maop);
  if (rhs_op.isConstant() == true)
    return true; // Trivial case.
  if (const auto rhs_fop = rhs_op.as<FunctionOperand>())
    return equalise(rhs_fop->getFunction().getRaw(), lhs);
  LOG(ERROR) << "Unexpected operand.";
  return false;
}

bool
racehog::Equaliser::
equalise(const Function* lhs,
         CallContext&    rhs_context,
         const Operand&  rhs_op) {
  // Roughly ordered by expected frequency of occurence.
  if (const auto rhs_maop = rhs_op.as<MemoryAtomOperand>())
    return forwardMemoryAtomOperand(lhs, rhs_context, rhs_maop);
  if (rhs_op.isConstant() == true)
    return true; // Trivial, though ... weird?
  // Note that two Function objects cannot be equalised (obviously).
  LOG(ERROR) << "Unexpected operand.";
  return false;
}

bool
racehog::Equaliser::
equalise(CallContext&   lhs_context,
         const Operand& lhs_op,
         CallContext&   rhs_context,
         const Operand& rhs_op) {
  if (const auto lhs_maop = lhs_op.as<MemoryAtomOperand>())
    return equalise(
      env.getMemoryAtom(lhs_context, *lhs_maop),
      rhs_context,
      rhs_op);
  if (const auto lhs_fop = lhs_op.as<FunctionOperand>())
    return equalise(
      lhs_fop->getFunction().getRaw(),
      rhs_context,
      rhs_op);
  LOG(ERROR) << "Unexpected operand.";
  return false;
}
