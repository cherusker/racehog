#ifndef STEENX_PROGRAM_OPERAND_GLOBALMEMORYATOMOPERAND_H_
#define STEENX_PROGRAM_OPERAND_GLOBALMEMORYATOMOPERAND_H_

#include "SteenX/Program/Operand/MemoryAtomOperand.h"

namespace racehog {
  
  /// Describes a global memory location.

  class GlobalMemoryAtomOperand final
    : public MemoryAtomOperand {

  public:

    static constexpr Type operand_type = Type::GlobalMemoryAtom;

    GlobalMemoryAtomOperand(MemoryAtomId id     = 0,
                            bool         is_ptr = false)
      : MemoryAtomOperand(operand_type, id, is_ptr) {}

    /// `Gid`.

    void
    print(UserStream&        stream,
          const std::string& indent = "") const override {
      stream << indent
             << (isPointer() == true ? "*" : "")
             << stream.setFormat(UserStream::Format::Variable)
             << "G" << getMemoryAtomId()
             << stream.resetFormat();
    }
  };
}

#endif // STEENX_PROGRAM_OPERAND_GLOBALMEMORYATOMOPERAND_H_
