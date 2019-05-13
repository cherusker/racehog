#ifndef STEENX_PROGRAM_INSTRUCTION_INSTRUCTION_H_
#define STEENX_PROGRAM_INSTRUCTION_INSTRUCTION_H_

#include <boost/core/noncopyable.hpp>

#include "SteenX/Program/Metadata/MetadataReference.h"
#include "SteenX/Program/Operand/Operands.h"
#include "Support/derivables.h"
#include "Support/range_iterator.h"
#include "Support/Stream/Printable.h"
#include "Support/Stream/Serialisable.h"

namespace racehog {
  
  /// Base class of all instructions.

  class Instruction
    : public  Printable,
      public  Serialisable,
      private boost::noncopyable {
    
    typedef derivables<Operand,
                       ConstantOperand,
                       FunctionOperand,
                       GlobalMemoryAtomOperand,
                       LocalMemoryAtomOperand> Operands;

  public:

    typedef Stream::InstructionType Type;
    typedef Stream::InstructionId   Id;

    typedef MetadataReference<Metadata> MetadataReference;

    typedef range_iterator<Operands::iterator>       OperandsRange;
    typedef range_iterator<Operands::const_iterator> ConstOperandsRange;

    virtual
    ~Instruction(void) = default;

    bool
    readFromStream(InputStream& stream) override;
    
    bool
    writeToStream(OutputStream& stream) const override;

    Type
    getType(void) const {
      return type;
    }

    const MetadataReference&
    getMetadata(void) const {
      return meta;
    }

    void
    setMetadata(const MetadataReference& meta) {
      this->meta = meta;
    }

    /// No bounds check;  know what you request.

    Operand::Id
    getNumOperands(void) const {
      return ops.size();
    }

    /// Use sparingly;  use high-level accessors where possible.
    
    const Operand&
    getOperand(Operand::Id id) const {
      return ops[id];
    }

    /// Use sparingly and with great caution.
    
    OperandsRange
    getOperands(void) {
      return OperandsRange(ops);
    }
    
    /// Use sparingly.
    
    ConstOperandsRange
    getOperands(void) const {
      return ConstOperandsRange(ops);
    }

    void
    append(const Operand& op);

    /// Returns the newly appended Operand.

    template<class    OperandType,
             class... Args>
    OperandType&
    append(Args&&... args) {
      return ops.emplace_back<OperandType>(std::forward<Args>(args)...);
    }

  protected:

    Instruction(Type type)
      : type(type) {}

    Instruction(Type                     type,
                const MetadataReference& meta)
      : type(type),
        meta(meta) {}

  private:

    // TODO:  Store X (5?) operands directy within the class and  avoid dynamic
    //   allocation for those?

    Type              type;
    MetadataReference meta;
    Operands          ops;
  };
}

#endif // STEENX_PROGRAM_INSTRUCTION_INSTRUCTION_H_
