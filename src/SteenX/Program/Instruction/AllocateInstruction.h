#ifndef STEENX_PROGRAM_INSTRUCTION_ALLOCATEINSTRUCTION_H_
#define STEENX_PROGRAM_INSTRUCTION_ALLOCATEINSTRUCTION_H_

#include "SteenX/Program/Instruction/ValueInstructionBase.h"
#include "Support/Stream/Stream.h"

namespace racehog {
  
  /// `value = allocate size`.

  class AllocateInstruction final
    : public ValueInstructionBase {

  public:

    typedef Stream::MemoryAtomId MemoryAtomId;

    AllocateInstruction(void)
      : ValueInstructionBase(Instruction::Type::Allocate) {}

    AllocateInstruction(const MetadataReference& meta,
                        MemoryAtomId             size = 0)
      : ValueInstructionBase(Instruction::Type::Allocate, meta),
        size(size) {}

    AllocateInstruction(const MetadataReference& meta,
                        const Operand&           val,
                        MemoryAtomId             size = 0)
      : ValueInstructionBase(Instruction::Type::Allocate, meta, val),
        size(size) {}

    bool
    readFromStream(InputStream& stream) override;

    bool
    writeToStream(OutputStream& stream) const override;

    MemoryAtomId
    getSize(void) const {
      return size;
    }

    /// `value = allocate size`.

    void
    print(UserStream&        stream,
          const std::string& indent = "") const override;

    MemoryAtomId size = 0; 
  };
}

#endif // STEENX_PROGRAM_INSTRUCTION_ALLOCATEINSTRUCTION_H_
