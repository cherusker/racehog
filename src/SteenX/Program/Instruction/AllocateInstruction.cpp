#include "SteenX/Program/Instruction/AllocateInstruction.h"

bool
racehog::AllocateInstruction::
readFromStream(InputStream& stream) {
  RH_ISTREAM_READUP(stream, Instruction);
  size = stream.readMemoryAtomId();
  return stream.allReadsGood();
}

bool
racehog::AllocateInstruction::
writeToStream(OutputStream& stream) const {
  RH_OSTREAM_WRITEUP(stream, Instruction);
  (void) stream.writeMemoryAtomId(size);
  return stream.good();
}

void
racehog::AllocateInstruction::
print(UserStream&        stream,
      const std::string& indent) const {
  (void) getValue().print(stream, indent);
  stream << " = "
         << stream.setFormat(UserStream::Format::Keyword)
         << "allocate"
         << stream.resetFormat()
         << " "
         << size;
  (void) getMetadata().print(stream, " ");
}
