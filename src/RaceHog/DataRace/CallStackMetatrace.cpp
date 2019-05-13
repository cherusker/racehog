#include "RaceHog/DataRace/CallStackMetatrace.h"

#include <boost/range/adaptor/reversed.hpp>

void
racehog::CallStackMetatrace::
print(UserStream&        stream,
      const std::string& indent) const {
  bool is_first = true;
  for (const auto& frame : boost::adaptors::reverse(frames)) {
    // CallStackMetaframe cannot inherit from Printable as it's used in
    //   turbovector and therefore cannot have virtual functions + a vtable.
    if (is_first == true)
      is_first = false;
    else
      stream << "\n";
    stream << indent;
    auto func = frame.getFunction();
    if (func == nullptr)
      stream << "<unknown function> ";
    else
      stream << stream.setFormat(UserStream::Format::String)
             << *func->getName()
             << stream.resetFormat()
             << " ";
    auto loc = frame.getLocation();
    if (loc == nullptr)
      stream << "<unknown location>";
    else
      (void) loc->print(stream);
  }
}
