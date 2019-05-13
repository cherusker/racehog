#include "RaceHog/DataRace/DataRaceAccess.h"

void
racehog::DataRaceAccess::
print(UserStream&        stream,
      const std::string& indent) const {
  stream << indent
         << stream.setForeground(UserStream::Colour::Red);
  switch (mode) {
  case Mode::Read:  stream << "Read";  break;
  case Mode::Write: stream << "Write"; break;
  default:          stream << "Unknown";
  }
  stream << stream.resetForeground()
         << " access by thread "
         << stream.setForeground(UserStream::Colour::Blue)
         << "T" << tid
         << stream.resetForeground();
  if (trace.getNumFrames() != 0) {
    stream << ":\n";
    (void) trace.print(stream, indent + "  ");
  }
}
