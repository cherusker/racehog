#ifndef STEENX_PROGRAM_INSTRUCTION_REFERENCEINSTRUCTION_H_
#define STEENX_PROGRAM_INSTRUCTION_REFERENCEINSTRUCTION_H_

#include "SteenX/Program/Instruction/Instruction.h"
#include "Support/Stream/Stream.h"

namespace racehog {
  
  /// `value -> base+/-offset[repeat x spread]`.
  /**
   * ReferenceInstruction is forged to mirror everthing that LLVM's
   * `getelementptr` throws at us;  including dynamic range accesses.
   * The presentation is similar to x86's `lea` instruction.
   * In addition, ReferenceInstruction can also handle C-style references.
   */

  class ReferenceInstruction final
    : public Instruction {

  public:

    typedef Stream::MemoryAtomId            MemoryAtomId;
    typedef Stream::MemoryAtomPointerOffset PointerOffset;

    ReferenceInstruction(void)
      : Instruction(Instruction::Type::Reference) {}

    ReferenceInstruction(const MetadataReference& meta,
                         PointerOffset            offset = 0,
                         MemoryAtomId             repeat = 0,
                         MemoryAtomId             spread = 0)
      : Instruction(Instruction::Type::Reference, meta),
        offset(offset),
        repeat(repeat),
        spread(spread) {}

    bool
    readFromStream(InputStream& stream) override;

    bool
    writeToStream(OutputStream& stream) const override;

    /// Instruction::getOperand(0).

    const Operand&
    getValue(void) const {
      return getOperand(val_id);
    }

    /// Instruction::getOperand(1).

    const Operand&
    getBase(void) const {
      return getOperand(base_id);
    }

    PointerOffset
    getBaseOffset(void) const {
      return offset;
    }

    MemoryAtomId
    getRangeRepeat(void) const {
      return repeat;
    }

    MemoryAtomId
    getRangeSpread(void) const {
      return spread;
    }

    /// `value -> base+/-offset[repeat x spread]`.

    void
    print(UserStream&        stream,
          const std::string& indent = "") const override;

  private:

    static constexpr Operand::Id val_id  = 0;
    static constexpr Operand::Id base_id = 1;

    PointerOffset offset = 0;
    MemoryAtomId  repeat = 0; 
    MemoryAtomId  spread = 0;
  };
}

#endif // STEENX_PROGRAM_INSTRUCTION_REFERENCEINSTRUCTION_H_
