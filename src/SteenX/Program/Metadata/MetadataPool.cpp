#include "SteenX/Program/Metadata/MetadataPool.h"

#include "Support/Macros.h"

bool
racehog::MetadataPool::
readFromStream(InputStream& stream) {
  RH_ISTREAM_READ(stream, MetadataSubpool, metadata);
  RH_ISTREAM_READ(stream, MetadataSubpool, namedmetadata);
  return true;
}

bool
racehog::MetadataPool::
writeToStream(OutputStream& stream) const {
  RH_OSTREAM_WRITE(stream, MetadataSubpool, metadata);
  RH_OSTREAM_WRITE(stream, MetadataSubpool, namedmetadata);
  return true;
}

void
racehog::MetadataPool::
print(UserStream&        stream,
      const std::string& indent) const {
  stream << indent << "Metadata:";
  if (metadata.empty() == true) {
    stream << " none";
  } else {
    stream << "\n";
    (void) metadata.print(stream, indent);
  }
  stream << "\n" << indent << "Named metadata:";
  if (namedmetadata.empty() == true) {
    stream << " none";
  } else {
    stream << "\n";
    (void) namedmetadata.print(stream, indent);
  }
}
