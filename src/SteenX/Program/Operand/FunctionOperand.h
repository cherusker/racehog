#ifndef STEENX_PROGRAM_OPERAND_FUNCTIONOPERAND_H_
#define STEENX_PROGRAM_OPERAND_FUNCTIONOPERAND_H_

#include "SteenX/Program/FunctionReference.h"
#include "SteenX/Program/Operand/Operand.h"

namespace racehog {
  
  /// Function pointer.

  class FunctionOperand final
    : public Operand {

  public:

    static constexpr Type operand_type = Type::Function;

    FunctionOperand(void)
      : Operand(operand_type) {}

    FunctionOperand(const FunctionReference& func)
      : Operand(operand_type),
        func(func) {}

    FunctionOperand(const FunctionOperand& other)
      : Operand(other),
        func(other.func) {}

    bool
    readFromStream(InputStream& stream) override;

    bool
    writeToStream(OutputStream& stream) const override;

    const FunctionReference&
    getFunction(void) const {
      return func;
    }

    void
    setFunction(const FunctionReference& func) {
      this->func = func;
    }

    /// `Fid`.

    void
    print(UserStream&        stream,
          const std::string& indent = "") const override {
      stream << indent
             << stream.setFormat(UserStream::Format::Variable)
             << "F" << func.getId()
             << stream.resetFormat();
    }

  private:

    FunctionReference func;
  };
}

#endif // STEENX_PROGRAM_OPERAND_FUNCTIONOPERAND_H_
