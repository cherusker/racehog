#include "SteenX/Program/Instruction/EqualiseInstruction.h"

void
racehog::EqualiseInstruction::
print(UserStream&        stream,
      const std::string& indent) const {
  (void) getLeftValue().print(stream, indent);
  (void) getRightValue().print(stream, " = ");
  (void) getMetadata().print(stream, " ");
}
