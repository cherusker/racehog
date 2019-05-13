#include "SteenX/VM/Memory/MemoryAccess.h"

#include <utility> // std::swap

bool
racehog::MemoryAccess::
racesWith(MemoryAccess& other) {
  if (this->getMode() == Mode::Read &&
      other.getMode() == Mode::Read)
    return false;
  auto t1 = this->thread->getResponsibleThread();
  auto t2 = other.thread->getResponsibleThread();
  if (t1 == t2)
    return false;
  // Make sure that `t1` happened before `t2`:
  if (this->time > other.time)
    (void) std::swap(t1, t2);
  if (t1->isAncestorOf(t2) == true)
    return false;
  return true;
}

void
racehog::MemoryAccess::
print(UserStream&        stream,
      const std::string& indent) const {
  stream << indent
         << stream.setFormat(UserStream::Format::Variable);
  switch (getMode()) {
  case Mode::Read:  stream << "R"; break;
  case Mode::Write: stream << "W"; break;
  default:          stream << "X";
  }
  stream << stream.resetFormat()
         << "@"
         << stream.setFormat(UserStream::Format::Variable)
         << time
         << stream.resetFormat()
         << "[";
  (void) thread->printId(stream);
  if (locks.size() != 0) {
    bool is_first = true;
    for (const auto& l : locks) {
      if (is_first == true) {
        stream << ":";
        is_first = false;
      } else {
        stream << ",";
      }
      stream << stream.setFormat(UserStream::Format::Variable)
             << "M" << l.first
             << stream.resetFormat();
    }
  }
  stream << "]";
}
