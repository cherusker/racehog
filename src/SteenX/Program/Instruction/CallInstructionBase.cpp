#include "SteenX/Program/Instruction/CallInstructionBase.h"

void
racehog::CallInstructionBase::
print(UserStream&        stream,
      const std::string& indent) const {
  stream << indent;
  (void) getFunction().print(stream);
  stream << "(";
  bool is_first = true;
  for (const auto& arg : getArguments()) {
    if (is_first == true)
      is_first = false;
    else
      stream << ", ";
    (void) arg.print(stream);
  }
  stream << ")";
}
