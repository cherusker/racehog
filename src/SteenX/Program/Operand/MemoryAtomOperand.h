#ifndef STEENX_PROGRAM_OPERAND_MEMORYATOMOPERAND_H_
#define STEENX_PROGRAM_OPERAND_MEMORYATOMOPERAND_H_

#include "SteenX/Program/Operand/Operand.h"

namespace racehog {
  
  /// Base Operand for LocalMemoryOperand and GlobalMemoryOperand.

  class MemoryAtomOperand
    : public Operand {

  public:

    typedef Stream::MemoryAtomId            MemoryAtomId;
    typedef Stream::MemoryAtomPointerOffset MemoryAtomPointerOffset;

    static constexpr Type operand_type = Type::MemoryAtom;

    bool
    readFromStream(InputStream& stream) override;

    bool
    writeToStream(OutputStream& stream) const override;

    MemoryAtomId
    getMemoryAtomId(void) const {
      return id;
    }

    bool
    isPointer(void) const {
      return is_ptr;
    }

  protected:

    MemoryAtomOperand(Type         type,
                      MemoryAtomId id     = 0,
                      bool         is_ptr = false)
      : Operand(type),
        id(id),
        is_ptr(is_ptr) {
      (void) addType(operand_type);
    }

    MemoryAtomOperand(const MemoryAtomOperand& other)
      : Operand(other),
        id(other.id),
        is_ptr(other.is_ptr) {}

  private:

    MemoryAtomId id;
    bool         is_ptr;
  };
}

#endif // STEENX_PROGRAM_OPERAND_MEMORYATOMOPERAND_H_
