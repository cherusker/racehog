#ifndef STEENX_PROGRAM_OPERAND_LOCALMEMORYATOMOPERAND_H_
#define STEENX_PROGRAM_OPERAND_LOCALMEMORYATOMOPERAND_H_

#include "SteenX/Program/Operand/MemoryAtomOperand.h"

namespace racehog {
  
  /// Describes a (function) local memory location.

  class LocalMemoryAtomOperand final
    : public MemoryAtomOperand {

  public:

    static constexpr Type operand_type = Type::LocalMemoryAtom;

    LocalMemoryAtomOperand(MemoryAtomId id     = 0,
                           bool         is_ptr = false)
      : MemoryAtomOperand(operand_type, id, is_ptr) {}

    /// `Lid`.

    void
    print(UserStream&        stream,
          const std::string& indent = "") const override {
      stream << indent
             << (isPointer() == true ? "*" : "")
             << stream.setFormat(UserStream::Format::Variable)
             << "L" << getMemoryAtomId()
             << stream.resetFormat();
    }
  };
}

#endif // STEENX_PROGRAM_OPERAND_LOCALMEMORYATOMOPERAND_H_
