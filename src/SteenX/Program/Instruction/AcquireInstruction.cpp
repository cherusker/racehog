#include "SteenX/Program/Instruction/AcquireInstruction.h"

void
racehog::AcquireInstruction::
print(UserStream&        stream,
      const std::string& indent) const {
  stream << indent
         << stream.setFormat(UserStream::Format::Keyword)
         << "acquire"
         << stream.resetFormat();
  (void) getLock().print(stream, " ");
  (void) getMetadata().print(stream, " ");
}
