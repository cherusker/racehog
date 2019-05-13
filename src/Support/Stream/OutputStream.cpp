#include "Support/Stream/OutputStream.h"

bool
racehog::OutputStream::
writeString(const std::string& str) {
  if (stream.write(str.c_str(), str.size()))
    return true;
  LOG(ERROR) << "std::ostream::write failed";
  return false;
}
