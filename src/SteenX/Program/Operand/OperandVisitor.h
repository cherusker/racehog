#ifndef STEENX_PROGRAM_OPERAND_OPERANDVISITOR_H_
#define STEENX_PROGRAM_OPERAND_OPERANDVISITOR_H_

#define RH_OPERANDVISITOR_CASE(TYPE)            \
  case Operand::Type::TYPE:                     \
  return static_cast<VisitorImpl*>(this)        \
  ->visit ## TYPE ## Operand(                   \
    static_cast<TYPE ## Operand&>(op));

#define RH_OPERANDVISITOR_CONST_CASE(TYPE)      \
  case Operand::Type::TYPE:                     \
  return static_cast<VisitorImpl*>(this)        \
  ->visit ## TYPE ## Operand(                   \
    static_cast<const TYPE ## Operand&>(op));

#define RH_OPERANDVISITOR_DELEGATE(TYPE)        \
  do {                                          \
    return static_cast<VisitorImpl*>(this)      \
      ->visit ## TYPE(op);                      \
  } while (0)

#include "SteenX/Program/Operand/Operands.h"
#include "Support/Macros.h"

namespace racehog {

  /// Static polymorphism Operand visitor.
  /**
   * See InstructionVisitor.
   */

  template<class VisitorImpl,
           class ReturnType = void>
  class OperandVisitor {

  public:

    ReturnType
    dispatch(Operand& op) {
      switch (op.getType()) {
        RH_OPERANDVISITOR_CASE(Constant);
        RH_OPERANDVISITOR_CASE(Function);
        RH_OPERANDVISITOR_CASE(GlobalMemoryAtom);
        RH_OPERANDVISITOR_CASE(LocalMemoryAtom);
      default:
        LOG(ERROR) << "Unexpected operand type.";
        RH_OPERANDVISITOR_DELEGATE(Operand);
      }
    }

    ReturnType
    dispatch(const Operand& op) {
      switch (op.getType()) {
        RH_OPERANDVISITOR_CONST_CASE(Constant);
        RH_OPERANDVISITOR_CONST_CASE(Function);
        RH_OPERANDVISITOR_CONST_CASE(GlobalMemoryAtom);
        RH_OPERANDVISITOR_CONST_CASE(LocalMemoryAtom);
      default:
        LOG(ERROR) << "Unexpected instruction type.";
        RH_OPERANDVISITOR_DELEGATE(Operand);
      }
    }

    void
    visitOperand(Operand& op) {
      RH_UNUSED(op);
    }

    void
    visitOperand(const Operand& op) {
      RH_UNUSED(op);
    }

    ReturnType
    visitConstantOperand(ConstantOperand& op) {
      RH_OPERANDVISITOR_DELEGATE(Operand);
    }

    ReturnType
    visitConstantOperand(const ConstantOperand& op) {
      RH_OPERANDVISITOR_DELEGATE(Operand);
    }

    ReturnType
    visitFunctionOperand(FunctionOperand& op) {
      RH_OPERANDVISITOR_DELEGATE(Operand);
    }

    ReturnType
    visitFunctionOperand(const FunctionOperand& op) {
      RH_OPERANDVISITOR_DELEGATE(Operand);
    }

    ReturnType
    visitGlobalMemoryAtomOperand(GlobalMemoryAtomOperand& op) {
      RH_OPERANDVISITOR_DELEGATE(Operand);
    }

    ReturnType
    visitGlobalMemoryAtomOperand(const GlobalMemoryAtomOperand& op) {
      RH_OPERANDVISITOR_DELEGATE(Operand);
    }

    ReturnType
    visitLocalMemoryAtomOperand(LocalMemoryAtomOperand& op) {
      RH_OPERANDVISITOR_DELEGATE(Operand);
    }

    ReturnType
    visitLocalMemoryAtomOperand(const LocalMemoryAtomOperand& op) {
      RH_OPERANDVISITOR_DELEGATE(Operand);
    }
  };
}

#endif // STEENX_PROGRAM_OPERAND_OPERANDVISITOR_H_
