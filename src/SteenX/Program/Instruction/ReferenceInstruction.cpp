#include "SteenX/Program/Instruction/ReferenceInstruction.h"

bool
racehog::ReferenceInstruction::
readFromStream(InputStream& stream) {
  RH_ISTREAM_READUP(stream, Instruction);
  offset = stream.readMemoryAtomPointerOffset();
  repeat = stream.readMemoryAtomId();
  spread = stream.readMemoryAtomId();
  return stream.allReadsGood();
}

bool
racehog::ReferenceInstruction::
writeToStream(OutputStream& stream) const {
  RH_OSTREAM_WRITEUP(stream, Instruction);
  (void) stream.writeMemoryAtomPointerOffset(offset);
  (void) stream.writeMemoryAtomId(repeat);
  (void) stream.writeMemoryAtomId(spread);
  return stream.good();
}

void
racehog::ReferenceInstruction::
print(UserStream&        stream,
      const std::string& indent) const {
  stream << indent;
  (void) getValue().print(stream);
  stream << " -> ";
  (void) getBase().print(stream);
  if (offset != 0)
    stream << std::showpos << offset << std::noshowpos;
  if (repeat != 0)
    stream << " [" << (repeat + 1) << " x " << spread << "]";
  (void) getMetadata().print(stream, " ");
}
