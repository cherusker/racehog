#ifndef STEENX_PROGRAM_INSTRUCTION_VALUEINSTRUCTIONBASE_H_
#define STEENX_PROGRAM_INSTRUCTION_VALUEINSTRUCTIONBASE_H_

#include "SteenX/Program/Instruction/Instruction.h"

namespace racehog {
  
  /// Base class for Instructions that have a single value.

  class ValueInstructionBase
    : public Instruction {

  public:

    /// Instruction::getOperand(0).

    const Operand&
    getValue(void) const {
      return getOperand(val_id);
    }

  protected:

    ValueInstructionBase(Instruction::Type type)
      : Instruction(type) {}

    ValueInstructionBase(Instruction::Type        type,
                         const MetadataReference& meta)
      : Instruction(type, meta) {}

    ValueInstructionBase(Instruction::Type        type,
                         const MetadataReference& meta,
                         const Operand&           val)
      : Instruction(type, meta) {
      (void) append(val);
    }

  private:

    static constexpr Operand::Id val_id = 0;
  };
}

#endif // STEENX_PROGRAM_INSTRUCTION_VALUEINSTRUCTIONBASE_H_
