#ifndef STEENX_PROGRAM_INSTRUCTION_LOCKINSTRUCTIONBASE_H_
#define STEENX_PROGRAM_INSTRUCTION_LOCKINSTRUCTIONBASE_H_

#include "SteenX/Program/Instruction/Instruction.h"

namespace racehog {
  
  /// Base class for Instructions that have a single value.

  class LockInstructionBase
    : public Instruction {

  public:

    /// Instruction::getOperand(0).

    const Operand&
    getLock(void) const {
      return getOperand(lock_id);
    }

  protected:

    LockInstructionBase(Instruction::Type type)
      : Instruction(type) {}

    LockInstructionBase(Instruction::Type        type,
                        const MetadataReference& meta)
      : Instruction(type, meta) {}

    LockInstructionBase(Instruction::Type        type,
                        const MetadataReference& meta,
                        const Operand&           lock)
      : Instruction(type, meta) {
      (void) append(lock);
    }

  private:

    static constexpr Operand::Id lock_id = 0;
  };
}

#endif // STEENX_PROGRAM_INSTRUCTION_LOCKINSTRUCTIONBASE_H_
