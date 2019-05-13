#include "SteenX/Program/Operand/MemoryAtomOperand.h"

bool
racehog::MemoryAtomOperand::
readFromStream(InputStream& stream) {
  RH_ISTREAM_READUP(stream, Operand);
  id     = stream.readMemoryAtomId();
  is_ptr = stream.readBool();
  return stream.allReadsGood();
}

bool
racehog::MemoryAtomOperand::
writeToStream(OutputStream& stream) const {
  RH_OSTREAM_WRITEUP(stream, Operand);
  (void) stream.writeMemoryAtomId(id);
  (void) stream.writeBool(is_ptr);
  return stream.good();
}
