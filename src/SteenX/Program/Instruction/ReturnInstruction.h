#ifndef STEENX_PROGRAM_INSTRUCTION_RETURNINSTRUCTION_H_
#define STEENX_PROGRAM_INSTRUCTION_RETURNINSTRUCTION_H_

#include "SteenX/Program/Instruction/ReturnValueInstructionBase.h"

namespace racehog {
  
  /// `return retval`.

  class ReturnInstruction final
    : public ReturnValueInstructionBase {

  public:

    ReturnInstruction(void)
      : ReturnValueInstructionBase(Instruction::Type::Return) {}

    ReturnInstruction(const MetadataReference& meta)
      : ReturnValueInstructionBase(Instruction::Type::Return, meta) {}

    /// `return retval`.

    void
    print(UserStream&        stream,
          const std::string& indent = "") const override;
  };
}

#endif // STEENX_PROGRAM_INSTRUCTION_RETURNINSTRUCTION_H_
