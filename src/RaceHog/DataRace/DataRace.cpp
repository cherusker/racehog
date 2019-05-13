#include "RaceHog/DataRace/DataRace.h"

void
racehog::DataRace::
print(UserStream&        stream,
      const std::string& indent) const {
  (void) getLeftAccess().print(stream, indent);
  stream << "\n";
  (void) getRightAccess().print(stream, indent);
  if (locks.size() == 0)
    return;
  stream << "\n" << indent
         << stream.setForeground(UserStream::Colour::Red)
         << "Protection"
         << stream.resetForeground()
         << ":";
  size_t id = 0;
  for (const auto& l : locks) {
    stream << "\n" << indent << "  Mutex " << (++id) << ":\n";
    (void) printLock(
      stream, indent + "    ", getLeftAccess(), l.getLeftLock());
    stream << "\n";
    (void) printLock(
      stream, indent + "    ", getRightAccess(), l.getRightLock());
  }
}

void
racehog::DataRace::
printLock(UserStream&           stream,
          const std::string&    indent,
          const DataRaceAccess& access,
          const DataRaceLock&   lock) const {
  stream << indent << "Acquired by "
         << stream.setForeground(UserStream::Colour::Blue)
         << "T" << access.getThreadId()
         << stream.resetForeground()
         << ":";
  for (const auto& l : lock.getLocations()) {
    if (l == nullptr) {
      stream << "\n" << indent << "  <unknown location>";
    } else {
      stream << "\n";
      (void) l->print(stream, indent + "  ");
    }
  }
}
