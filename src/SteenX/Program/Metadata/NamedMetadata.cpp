#include "SteenX/Program/Metadata/NamedMetadata.h"

#include "Support/Macros.h"

bool
racehog::NamedMetadata::
readFromStream(InputStream& stream) {
  RH_ISTREAM_READUP(stream, Metadata);
  RH_ISTREAM_READ(stream, StringReference, name);
  return true;
}

bool
racehog::NamedMetadata::
writeToStream(OutputStream& stream) const {
  RH_OSTREAM_WRITEUP(stream, Metadata);
  RH_OSTREAM_WRITE(stream, StringReference, name);
  return true;
}

void
racehog::NamedMetadata::
print(UserStream&        stream,
      const std::string& indent) const {
  stream << indent
         << stream.setFormat(UserStream::Format::String)
         << "\"" << *getName() << "\""
         << stream.resetFormat()
         << " in ";
  (void) Metadata::print(stream);
}
