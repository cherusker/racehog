#ifndef STEENX_PROGRAM_INSTRUCTION_JOININSTRUCTION_H_
#define STEENX_PROGRAM_INSTRUCTION_JOININSTRUCTION_H_

#include "SteenX/Program/Instruction/ReturnValueInstructionBase.h"

namespace racehog {
  
  /// `retval = join thread`.

  class JoinInstruction final
    : public ReturnValueInstructionBase {

  public:

    JoinInstruction(void)
      : ReturnValueInstructionBase(Instruction::Type::Join) {}

    JoinInstruction(const MetadataReference& meta)
      : ReturnValueInstructionBase(Instruction::Type::Join, meta) {}

    /// Instruction::getOperand(1).

    const Operand&
    getThreadPointer(void) const {
      return getOperand(thread_id);
    }

    /// `retval = join thread`.

    void
    print(UserStream&        stream,
          const std::string& indent = "") const override;

  private:

    static constexpr Operand::Id thread_id = 1;
  };
}

#endif // STEENX_PROGRAM_INSTRUCTION_JOININSTRUCTION_H_
