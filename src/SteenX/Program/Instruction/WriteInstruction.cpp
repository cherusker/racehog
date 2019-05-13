#include "SteenX/Program/Instruction/WriteInstruction.h"

void
racehog::WriteInstruction::
print(UserStream&        stream,
      const std::string& indent) const {
  stream << indent
         << stream.setFormat(UserStream::Format::Keyword)
         << "write"
         << stream.resetFormat();
  (void) getValue().print(stream, " ");
  (void) getMetadata().print(stream, " ");
}
