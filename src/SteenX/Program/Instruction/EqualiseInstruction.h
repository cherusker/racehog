#ifndef STEENX_PROGRAM_INSTRUCTION_EQUALISEINSTRUCTION_H_
#define STEENX_PROGRAM_INSTRUCTION_EQUALISEINSTRUCTION_H_

#include "SteenX/Program/Instruction/Instruction.h"

namespace racehog {
  
  /// `left = right`.

  class EqualiseInstruction final
    : public Instruction {

  public:

    EqualiseInstruction(void)
      : Instruction(Instruction::Type::Equalise) {}

    EqualiseInstruction(const MetadataReference& meta)
      : Instruction(Instruction::Type::Equalise, meta) {}

    /// Instruction::getOperand(0).

    const Operand&
    getLeftValue(void) const {
      return getOperand(left_id);
    }

    /// Instruction::getOperand(1).

    const Operand&
    getRightValue(void) const {
      return getOperand(right_id);
    }

    /// `left = right`.

    void
    print(UserStream&        stream,
          const std::string& indent = "") const override;

  private:
    
    static constexpr Operand::Id left_id  = 0;
    static constexpr Operand::Id right_id = 1;
  };
}

#endif // STEENX_PROGRAM_INSTRUCTION_EQUALISEINSTRUCTION_H_
