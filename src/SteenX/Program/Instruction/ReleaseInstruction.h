#ifndef STEENX_PROGRAM_INSTRUCTION_RELEASEINSTRUCTION_H_
#define STEENX_PROGRAM_INSTRUCTION_RELEASEINSTRUCTION_H_

#include "SteenX/Program/Instruction/LockInstructionBase.h"

namespace racehog {
  
  /// `release lock`.

  class ReleaseInstruction final
    : public LockInstructionBase {

  public:

    ReleaseInstruction(void)
      : LockInstructionBase(Instruction::Type::Release) {}

    ReleaseInstruction(const MetadataReference& meta)
      : LockInstructionBase(Instruction::Type::Release, meta) {}

    ReleaseInstruction(const MetadataReference& meta,
                       const Operand&           lock)
      : LockInstructionBase(Instruction::Type::Release, meta, lock) {}

    /// `release lock`.
    
    void
    print(UserStream&        stream,
          const std::string& indent = "") const override;
  };
}

#endif // STEENX_PROGRAM_INSTRUCTION_RELEASEINSTRUCTION_H_
