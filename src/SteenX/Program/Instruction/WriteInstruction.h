#ifndef STEENX_PROGRAM_INSTRUCTION_WRITEINSTRUCTION_H_
#define STEENX_PROGRAM_INSTRUCTION_WRITEINSTRUCTION_H_

#include "SteenX/Program/Instruction/ValueInstructionBase.h"

namespace racehog {
  
  /// `write value`.

  class WriteInstruction final
    : public ValueInstructionBase {

  public:

    WriteInstruction(void)
      : ValueInstructionBase(Instruction::Type::Write) {}

    WriteInstruction(const MetadataReference& meta)
      : ValueInstructionBase(Instruction::Type::Write, meta) {}

    WriteInstruction(const MetadataReference& meta,
                     const Operand&           val)
      : ValueInstructionBase(Instruction::Type::Write, meta, val) {}

    /// `write value`.

    void
    print(UserStream&        stream,
          const std::string& indent = "") const override;
  };
}

#endif // STEENX_PROGRAM_INSTRUCTION_WRITEINSTRUCTION_H_
