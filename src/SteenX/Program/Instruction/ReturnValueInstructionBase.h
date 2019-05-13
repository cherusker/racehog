#ifndef STEENX_PROGRAM_INSTRUCTION_RETURNVALUEINSTRUCTIONBASE_H_
#define STEENX_PROGRAM_INSTRUCTION_RETURNVALUEINSTRUCTIONBASE_H_

#include "SteenX/Program/Instruction/Instruction.h"

namespace racehog {
  
  /// Base class for dealing with a return value.

  class ReturnValueInstructionBase
    : public Instruction {

  public:

    /// Instruction::getOperand(0).

    const Operand&
    getReturnValue(void) const {
      return getOperand(retval_id);
    }

  protected:

    ReturnValueInstructionBase(Instruction::Type type)
      : Instruction(type) {}

    ReturnValueInstructionBase(Instruction::Type        type,
                               const MetadataReference& meta)
      : Instruction(type, meta) {}

  private:

    static constexpr Operand::Id retval_id = 0;
  };
}

#endif // STEENX_PROGRAM_INSTRUCTION_RETURNVALUEINSTRUCTIONBASE_H_
