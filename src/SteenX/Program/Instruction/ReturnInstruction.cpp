#include "SteenX/Program/Instruction/ReturnInstruction.h"

void
racehog::ReturnInstruction::
print(UserStream&        stream,
      const std::string& indent) const  {
  stream << indent
         << stream.setFormat(UserStream::Format::Keyword)
         << "return"
         << stream.resetFormat();
  (void) getReturnValue().print(stream, " ");
  (void) getMetadata().print(stream, " ");
}
