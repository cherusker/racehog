#ifndef STEENX_PROGRAM_INSTRUCTION_CALLINSTRUCTIONBASE_H_
#define STEENX_PROGRAM_INSTRUCTION_CALLINSTRUCTIONBASE_H_

#include "SteenX/Program/Instruction/Instruction.h"

namespace racehog {
  
  /// Base class for dealing with call sites.

  class CallInstructionBase
    : public Instruction {

  public:

    /// Instruction::getOperand(0).

    const Operand&
    getFunction(void) const {
      return getOperand(func_id);
    }

    Operand::Id
    getNumArguments(void) const {
      return getNumOperands() - num_static_ops;
    }

    /// Reduced version of Instruction::getOperands;  only function arguments.

    ConstOperandsRange
    getArguments(void) const {
      // TODO:  Does this compile to acceptible code or do we need a different
      // solution?
      auto range = getOperands();
      return ConstOperandsRange(range.begin() + num_static_ops, range.end());
    }

  protected:

    /**
     * \param num_static_ops Required to correctly calculate the offset of the
     *        function arguments in CallInstructionBase::getArguments.
     */

    CallInstructionBase(Instruction::Type type,
                        Operand::Id       num_static_ops = 1)
      : Instruction(type),
        num_static_ops(num_static_ops) {}

    CallInstructionBase(Instruction::Type        type,
                        const MetadataReference& meta,
                        Operand::Id              num_static_ops = 1)
      : Instruction(type, meta),
        num_static_ops(num_static_ops) {}

    /// `func(args...)`.

    void
    print(UserStream&        stream,
          const std::string& indent = "") const override;

  private:

    static constexpr Operand::Id func_id = 0;
    const Operand::Id            num_static_ops;
  };
}

#endif // STEENX_PROGRAM_INSTRUCTION_CALLINSTRUCTIONBASE_H_
