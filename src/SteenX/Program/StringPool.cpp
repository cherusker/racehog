#include "SteenX/Program/StringPool.h"

bool
racehog::StringPool::
readFromStream(InputStream& stream) {
  auto num_strs = stream.readStringId();
  while (num_strs-- > 0)
    (void) strs.push_back(stream.readString(stream.readStringLength()));
  return stream.allReadsGood();
}

bool
racehog::StringPool::
writeToStream(OutputStream& stream) const {
  (void) stream.writeStringId(strs.size());
  for (const auto& str : strs) {
    if (stream.writeStringLength(str.size()) == false ||
        stream.writeString(str)              == false)
      return false;
  }
  return stream.good();
}

void
racehog::StringPool::
print(UserStream&        stream,
      const std::string& indent) const {
  StringId id = 0;
  for (const auto& str : strs) {
    if (id > 0)
      stream << "\n";
    stream << indent << id++ << ": "
           << stream.setFormat(UserStream::Format::String)
           << "\"" << str << "\""
           << stream.resetFormat();
  }
}
