#ifndef STEENX_PROGRAM_INSTRUCTION_ACQUIREINSTRUCTION_H_
#define STEENX_PROGRAM_INSTRUCTION_ACQUIREINSTRUCTION_H_

#include "SteenX/Program/Instruction/LockInstructionBase.h"

namespace racehog {
  
  /// `acquire lock`.

  class AcquireInstruction final
    : public LockInstructionBase {

  public:

    AcquireInstruction(void)
      : LockInstructionBase(Instruction::Type::Acquire) {}

    AcquireInstruction(const MetadataReference& meta)
      : LockInstructionBase(Instruction::Type::Acquire, meta) {}

    AcquireInstruction(const MetadataReference& meta,
                       const Operand&           lock)
      : LockInstructionBase(Instruction::Type::Acquire, meta, lock) {}

    /// `acquire lock`.
    
    void
    print(UserStream&        stream,
          const std::string& indent = "") const override;
  };
}

#endif // STEENX_PROGRAM_INSTRUCTION_ACQUIREINSTRUCTION_H_
