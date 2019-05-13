#ifndef STEENX_PROGRAM_INSTRUCTION_FORKINSTRUCTION_H_
#define STEENX_PROGRAM_INSTRUCTION_FORKINSTRUCTION_H_

#include "SteenX/Program/Instruction/CallInstructionBase.h"

namespace racehog {
  
  /// `thread = fork func(args...)`.

  class ForkInstruction final
    : public CallInstructionBase {

  public:

    ForkInstruction(void)
      : CallInstructionBase(Instruction::Type::Fork, 2) {}

    ForkInstruction(const MetadataReference& meta)
      : CallInstructionBase(Instruction::Type::Fork, meta, 2) {}

    /// Instruction::getOperand(1).

    const Operand&
    getThreadPointer(void) const {
      return getOperand(thread_id);
    }

    /// `thread = fork func(args...)`.

    void
    print(UserStream&        stream,
          const std::string& indent = "") const override;

  private:

    static constexpr Operand::Id thread_id = 1;
  };
}

#endif // STEENX_PROGRAM_INSTRUCTION_FORKINSTRUCTION_H_
