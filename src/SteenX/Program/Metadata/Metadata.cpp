#include "SteenX/Program/Metadata/Metadata.h"

bool
racehog::Metadata::
readFromStream(InputStream& stream) {
  RH_ISTREAM_READ(stream, StringReference, dir);
  RH_ISTREAM_READ(stream, StringReference, filename);
  (void) setLine(stream.readLine());
  (void) setColumn(stream.readColumn());
  return stream.allReadsGood();
}

bool
racehog::Metadata::
writeToStream(OutputStream& stream) const {
  RH_OSTREAM_WRITE(stream, StringReference, dir);
  RH_OSTREAM_WRITE(stream, StringReference, filename);
  (void) stream.writeLine(getLine());
  (void) stream.writeColumn(getColumn());
  return stream.good();
}

void
racehog::Metadata::
print(UserStream&        stream,
      const std::string& indent) const {
  stream << indent
         << *getDirectory() << "/" << *getFilename()
         << ":" << line << ":" << col;
}
