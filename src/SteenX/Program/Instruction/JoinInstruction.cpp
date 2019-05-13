#include "SteenX/Program/Instruction/JoinInstruction.h"

void
racehog::JoinInstruction::
print(UserStream&        stream,
      const std::string& indent) const {
  (void) getReturnValue().print(stream, indent);
  stream << " = "
         << stream.setFormat(UserStream::Format::Keyword)
         << "join"
         << stream.resetFormat();
  (void) getThreadPointer().print(stream, " ");
  (void) getMetadata().print(stream, " ");
}
