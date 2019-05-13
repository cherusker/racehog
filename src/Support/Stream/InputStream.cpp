#include "Support/Stream/InputStream.h"

std::string
racehog::InputStream::
readString(StringLength len) {
  if (strbuf_len < len) {
    delete[] strbuf;
    strbuf = new char[len];
  }
  if (stream.read(strbuf, len))
    return std::string(strbuf, len);
  LOG(ERROR) << "std::istream::read failed";
  all_reads_good = false;
  return "";
}
