#include "SteenX/Program/Instruction/ReleaseInstruction.h"

void
racehog::ReleaseInstruction::
print(UserStream&        stream,
      const std::string& indent) const {
  stream << indent
         << stream.setFormat(UserStream::Format::Keyword)
         << "release"
         << stream.resetFormat();
  (void) getLock().print(stream, " ");
  (void) getMetadata().print(stream, " ");
}
