#include "SteenX/Program/Operand/FunctionOperand.h"

bool
racehog::FunctionOperand::
readFromStream(InputStream& stream) {
  RH_ISTREAM_READUP(stream, Operand);
  RH_ISTREAM_READ(stream, FunctionReference, func);
  return true;
}

bool
racehog::FunctionOperand::
writeToStream(OutputStream& stream) const {
  RH_OSTREAM_WRITEUP(stream, Operand);
  RH_OSTREAM_WRITE(stream, FunctionReference, func);
  return true;
}
