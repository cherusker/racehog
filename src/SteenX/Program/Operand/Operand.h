#ifndef STEENX_PROGRAM_OPERAND_OPERAND_H_
#define STEENX_PROGRAM_OPERAND_OPERAND_H_

#include <cstdint>

#include <boost/core/noncopyable.hpp>

#include "Support/Stream/Printable.h"
#include "Support/Stream/Serialisable.h"
#include "Support/Logging.h"

namespace racehog {
  
  /// Atomic part of Instructions (variables, constants, functions, ...).

  class Operand
    : public  Printable,
      public  Serialisable,
      private boost::noncopyable {

  public:

    typedef Stream::OperandType Type;
    typedef Stream::OperandId   Id;

    virtual
    ~Operand(void) {}

    /// Currently `nop` but could be populated one day.

    bool
    readFromStream(InputStream& stream) override {
      RH_UNUSED(stream);
      return true;
    }

    /// Currently `nop` but could be populated one day.

    bool
    writeToStream(OutputStream& stream) const override {
      RH_UNUSED(stream);
      return true;
    }

    /// Returns the leaf type (see Operand::hasType for details).

    Type
    getType(void) const {
      return leaf_t;
    }

    /// If Operands can have multiple types, use Operand::hasType.
    /**
     * For example, if we face `LocalMemoryAtomOperand op;` then
     * `op.hasType(Type::MemoryAtom) == true` and 
     * `op.hasType(Type::LocalMemoryAtom) == true` as well.
     * Beware, however, that `op.getType() == Type::LocalMemoryAtom` without any
     * trace of Type::MemoryAtom.
     */

    constexpr bool
    hasType(Type type) const {
      return ((full_t & (1 << static_cast<int>(type))) == 0) ? false : true;
    }

    /// Shortcut for Operand::getType with Type::Constant.

    bool
    isConstant(void) const {
      return getType() == Type::Constant;
    }

    /// Shortcut for Operand::getType with Type::Function.

    bool
    isFunction(void) const {
      return getType() == Type::Function;
    }

    /// Shortcut for Operand::hasType with Type::MemoryAtom.

    bool
    isMemoryAtom(void) const {
      return hasType(Type::MemoryAtom);
    }

    /// Shortcut for Operand::getType with Type::GlobalMemoryAtom.

    bool
    isGlobalMemoryAtom(void) const {
      return getType() == Type::GlobalMemoryAtom;
    }

    /// Shortcut for Operand::getType with Type::LocalMemoryAtom.

    bool
    isLocalMemoryAtom(void) const {
      return getType() == Type::LocalMemoryAtom;
    }

    /// Returns `nullptr` if cast goes wrong.

    template<class SubOperandType>
    SubOperandType*
    as(void) {
      return hasType(SubOperandType::operandType) == true ?
        static_cast<SubOperandType*>(this) : nullptr;
    }

    /// Returns `nullptr` if cast goes wrong.

    template<class SubOperandType>
    const SubOperandType*
    as(void) const {
      return hasType(SubOperandType::operand_type) == true ?
        static_cast<const SubOperandType*>(this) : nullptr;
    }

  protected:

    Operand(Type type = Type::Invalid)
      : leaf_t(type) {
      (void) addType(type);
    }

    Operand(const Operand& other)
      : leaf_t(other.leaf_t),
        full_t(other.full_t) {
      LOG(INFO) << "Copying Operand.";
    }

    Operand(Operand&& other)
      : leaf_t(other.leaf_t),
        full_t(other.full_t) {
      LOG(INFO) << "Moving Operand.";
    }

    /// Has to be set during construction of every _intermediate_ sub class.
    /**
     * This it is our way around efficient and safe downcasting without RTTI.
     * Remember that Operand:Operand(Type) calls Operand::addDeepType for the
     * given (leaf) Type automatically to avoid some boilerplate.
     */

    constexpr void
    addType(Type type) {
      full_t |= (1 << static_cast<int>(type));
    }

  private:

    /// See Operand::addDeepType.

    typedef uint8_t FullType; // 8 possible types.

    Type     leaf_t;
    FullType full_t = 0; ///< See Operand::addType.
  };
}

#endif // STEENX_PROGRAM_OPERAND_OPERAND_H_
