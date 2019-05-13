#ifndef STEENX_PROGRAM_INSTRUCTION_READINSTRUCTION_H_
#define STEENX_PROGRAM_INSTRUCTION_READINSTRUCTION_H_

#include "SteenX/Program/Instruction/ValueInstructionBase.h"

namespace racehog {
  
  /// `read value`.

  class ReadInstruction final
    : public ValueInstructionBase {

  public:

    ReadInstruction(void)
      : ValueInstructionBase(Instruction::Type::Read) {}

    ReadInstruction(const MetadataReference& meta)
      : ValueInstructionBase(Instruction::Type::Read, meta) {}

    ReadInstruction(const MetadataReference& meta,
                    const Operand&           val)
      : ValueInstructionBase(Instruction::Type::Read, meta, val) {}

    /// `read value`.

    void
    print(UserStream&        stream,
          const std::string& indent = "") const override;
  };
}

#endif // STEENX_PROGRAM_INSTRUCTION_READINSTRUCTION_H_
