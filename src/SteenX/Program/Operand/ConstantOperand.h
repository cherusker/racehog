#ifndef STEENX_PROGRAM_OPERAND_CONSTANTOPERAND_H_
#define STEENX_PROGRAM_OPERAND_CONSTANTOPERAND_H_

#include "SteenX/Program/Operand/Operand.h"

namespace racehog {
  
  /// Simple constant without further information.

  class ConstantOperand final
    : public Operand {

  public:

    static constexpr Type operand_type = Type::Constant;

    ConstantOperand(void)
      : Operand(operand_type) {}

    /// `C`.

    void
    print(UserStream&        stream,
          const std::string& indent = "") const override {
      stream << indent
             << stream.setFormat(UserStream::Format::Variable)
             << "C"
             << stream.resetFormat();
    }
  };
}

#endif // STEENX_PROGRAM_OPERAND_CONSTANTOPERAND_H_
