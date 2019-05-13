#ifndef STEENX_PROGRAM_INSTRUCTION_CALLINSTRUCTION_H_
#define STEENX_PROGRAM_INSTRUCTION_CALLINSTRUCTION_H_

#include "SteenX/Program/Instruction/CallInstructionBase.h"

namespace racehog {
  
  /// `retval = func(args...)`

  class CallInstruction final
    : public CallInstructionBase {

  public:

    CallInstruction(void)
      : CallInstructionBase(Instruction::Type::Call, 2) {}

    CallInstruction(const MetadataReference& meta)
      : CallInstructionBase(Instruction::Type::Call, meta, 2) {}

    /// Instruction::getOperand(1).

    const Operand&
    getReturnValue(void) const {
      return getOperand(retval_id);
    }

    /// `retval = func(args...)`.

    void
    print(UserStream&        stream,
          const std::string& indent = "") const override;

  private:

    static constexpr Operand::Id retval_id = 1;
  };
}

#endif // STEENX_PROGRAM_INSTRUCTION_CALLINSTRUCTION_H_
