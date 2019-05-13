#include "SteenX/Program/Instruction/CallInstruction.h"

void
racehog::CallInstruction::
print(UserStream&        stream,
      const std::string& indent) const {
  (void) getReturnValue().print(stream, indent);
  stream << " = "
         << stream.setFormat(UserStream::Format::Keyword)
         << "call"
         << stream.resetFormat();
  (void) CallInstructionBase::print(stream, " ");
  (void) getMetadata().print(stream, " ");
}
