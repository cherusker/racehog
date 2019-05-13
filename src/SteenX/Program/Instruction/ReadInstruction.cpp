#include "SteenX/Program/Instruction/ReadInstruction.h"

void
racehog::ReadInstruction::
print(UserStream&        stream,
      const std::string& indent) const {
  stream << indent
         << stream.setFormat(UserStream::Format::Keyword)
         << "read"
         << stream.resetFormat();
  (void) getValue().print(stream, " ");
  (void) getMetadata().print(stream, " ");
}
