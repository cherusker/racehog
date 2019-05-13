#ifndef STEENX_PROGRAM_INSTRUCTION_INSTRUCTIONVISITOR_H_
#define STEENX_PROGRAM_INSTRUCTION_INSTRUCTIONVISITOR_H_

#define RH_INSTRUCTIONVISITOR_CASE(TYPE)            \
  case Instruction::Type::TYPE:                     \
  return static_cast<InstructionVisitorImpl*>(this) \
  ->visit ## TYPE ## Instruction(                   \
    static_cast<TYPE ## Instruction&>(inst));

#define RH_INSTRUCTIONVISITOR_CONST_CASE(TYPE)      \
  case Instruction::Type::TYPE:                     \
  return static_cast<InstructionVisitorImpl*>(this) \
  ->visit ## TYPE ## Instruction(                   \
    static_cast<const TYPE ## Instruction&>(inst));

#define RH_INSTRUCTIONVISITOR_DELEGATE(TYPE)          \
  do {                                                \
    return static_cast<InstructionVisitorImpl*>(this) \
      ->visit ## TYPE(inst);                          \
  } while (0)

#include "SteenX/Program/Instruction/Instructions.h"
#include "Support/Macros.h"

namespace racehog {

  /// Static polymorphism Instruction visitor.
  /**
   * The overhead of `virtual` functions should be avoided in the VM execution
   * phase which is why InstructionVisitor works very similarly to LLVM's
   * InstVisitor:  it uses static polymorphism.  Use InstructionVisitor::visit
   * as entry function (`accept`) that will act as big `switch` statement and
   * overwrite needed visit* functions.
   *
   * \tparam InstructionVisitorImpl the deriving class has to be registered
   *                                correctly to make InstructionVisitor work.
   */

  template<class InstructionVisitorImpl,
           class ReturnType = void>
  class InstructionVisitor {

  public:
    
    ReturnType
    dispatch(Instruction& inst) {
      switch (inst.getType()) {
        RH_INSTRUCTIONVISITOR_CASE(Fork);
        RH_INSTRUCTIONVISITOR_CASE(Join);
        RH_INSTRUCTIONVISITOR_CASE(Acquire);
        RH_INSTRUCTIONVISITOR_CASE(Release);
        RH_INSTRUCTIONVISITOR_CASE(Call);
        RH_INSTRUCTIONVISITOR_CASE(Return);
        RH_INSTRUCTIONVISITOR_CASE(Reference);
        RH_INSTRUCTIONVISITOR_CASE(Equalise);
        RH_INSTRUCTIONVISITOR_CASE(Read);
        RH_INSTRUCTIONVISITOR_CASE(Write);
        RH_INSTRUCTIONVISITOR_CASE(Allocate);
      default:
        LOG(ERROR) << "Unexpected instruction type.";
        RH_INSTRUCTIONVISITOR_DELEGATE(Instruction);
      }
    }
    
    ReturnType
    dispatch(const Instruction& inst) {
      switch (inst.getType()) {
        RH_INSTRUCTIONVISITOR_CONST_CASE(Fork);
        RH_INSTRUCTIONVISITOR_CONST_CASE(Join);
        RH_INSTRUCTIONVISITOR_CONST_CASE(Acquire);
        RH_INSTRUCTIONVISITOR_CONST_CASE(Release);
        RH_INSTRUCTIONVISITOR_CONST_CASE(Call);
        RH_INSTRUCTIONVISITOR_CONST_CASE(Return);
        RH_INSTRUCTIONVISITOR_CONST_CASE(Reference);
        RH_INSTRUCTIONVISITOR_CONST_CASE(Equalise);
        RH_INSTRUCTIONVISITOR_CONST_CASE(Read);
        RH_INSTRUCTIONVISITOR_CONST_CASE(Write);
        RH_INSTRUCTIONVISITOR_CONST_CASE(Allocate);
      default:
        LOG(ERROR) << "Unexpected instruction type.";
        RH_INSTRUCTIONVISITOR_DELEGATE(Instruction);
      }
    }

    /// Has to be reimplemented in deriving class if return type `!= void`.

    void
    visitInstruction(Instruction& inst) {
      RH_UNUSED(inst);
    }

    /// Has to be reimplemented in deriving class if return type `!= void`.

    void
    visitInstruction(const Instruction& inst) {
      RH_UNUSED(inst);
    }

    ReturnType
    visitCallInstructionBase(CallInstructionBase& inst) {
      RH_INSTRUCTIONVISITOR_DELEGATE(Instruction);
    }

    ReturnType
    visitCallInstructionBase(const CallInstructionBase& inst) {
      RH_INSTRUCTIONVISITOR_DELEGATE(Instruction);
    }

    ReturnType
    visitForkInstruction(ForkInstruction& inst) {
      RH_INSTRUCTIONVISITOR_DELEGATE(CallInstructionBase);
    }

    ReturnType
    visitForkInstruction(const ForkInstruction& inst) {
      RH_INSTRUCTIONVISITOR_DELEGATE(CallInstructionBase);
    }

    ReturnType
    visitJoinInstruction(JoinInstruction& inst) {
      RH_INSTRUCTIONVISITOR_DELEGATE(Instruction);
    }

    ReturnType
    visitJoinInstruction(const JoinInstruction& inst) {
      RH_INSTRUCTIONVISITOR_DELEGATE(Instruction);
    }

    ReturnType
    visitAcquireInstruction(AcquireInstruction& inst) {
      RH_INSTRUCTIONVISITOR_DELEGATE(Instruction);
    }

    ReturnType
    visitAcquireInstruction(const AcquireInstruction& inst) {
      RH_INSTRUCTIONVISITOR_DELEGATE(Instruction);
    }

    ReturnType
    visitReleaseInstruction(ReleaseInstruction& inst) {
      RH_INSTRUCTIONVISITOR_DELEGATE(Instruction);
    }

    ReturnType
    visitReleaseInstruction(const ReleaseInstruction& inst) {
      RH_INSTRUCTIONVISITOR_DELEGATE(Instruction);
    }

    ReturnType
    visitCallInstruction(CallInstruction& inst) {
      RH_INSTRUCTIONVISITOR_DELEGATE(CallInstructionBase);
    }

    ReturnType
    visitCallInstruction(const CallInstruction& inst) {
      RH_INSTRUCTIONVISITOR_DELEGATE(CallInstructionBase);
    }

    ReturnType
    visitReturnInstruction(ReturnInstruction& inst) {
      RH_INSTRUCTIONVISITOR_DELEGATE(Instruction);
    }

    ReturnType
    visitReturnInstruction(const ReturnInstruction& inst) {
      RH_INSTRUCTIONVISITOR_DELEGATE(Instruction);
    }

    ReturnType
    visitReferenceInstruction(ReferenceInstruction& inst) {
      RH_INSTRUCTIONVISITOR_DELEGATE(Instruction);
    }

    ReturnType
    visitReferenceInstruction(const ReferenceInstruction& inst) {
      RH_INSTRUCTIONVISITOR_DELEGATE(Instruction);
    }

    ReturnType
    visitEqualiseInstruction(EqualiseInstruction& inst) {
      RH_INSTRUCTIONVISITOR_DELEGATE(Instruction);
    }

    ReturnType
    visitEqualiseInstruction(const EqualiseInstruction& inst) {
      RH_INSTRUCTIONVISITOR_DELEGATE(Instruction);
    }

    ReturnType
    visitReadInstruction(ReadInstruction& inst) {
      RH_INSTRUCTIONVISITOR_DELEGATE(Instruction);
    }

    ReturnType
    visitReadInstruction(const ReadInstruction& inst) {
      RH_INSTRUCTIONVISITOR_DELEGATE(Instruction);
    }

    ReturnType
    visitWriteInstruction(WriteInstruction& inst) {
      RH_INSTRUCTIONVISITOR_DELEGATE(Instruction);
    }

    ReturnType
    visitWriteInstruction(const WriteInstruction& inst) {
      RH_INSTRUCTIONVISITOR_DELEGATE(Instruction);
    }

    ReturnType
    visitAllocateInstruction(AllocateInstruction& inst) {
      RH_INSTRUCTIONVISITOR_DELEGATE(Instruction);
    }

    ReturnType
    visitAllocateInstruction(const AllocateInstruction& inst) {
      RH_INSTRUCTIONVISITOR_DELEGATE(Instruction);
    }
  };
}

#endif // STEENX_PROGRAM_INSTRUCTION_INSTRUCTIONVISITOR_H_
