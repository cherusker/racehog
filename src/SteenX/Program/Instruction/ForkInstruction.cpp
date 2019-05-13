#include "SteenX/Program/Instruction/ForkInstruction.h"

void
racehog::ForkInstruction::
print(UserStream&        stream,
      const std::string& indent) const {
  (void) getThreadPointer().print(stream, indent);
  stream << " = "
         << stream.setFormat(UserStream::Format::Keyword)
         << "fork"
         << stream.resetFormat();
  (void) CallInstructionBase::print(stream, " ");
  (void) getMetadata().print(stream, " ");
}
