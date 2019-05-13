#include "SteenX/VM/Thread.h"

racehog::Thread*
racehog::Thread::
getResponsibleThread(void) {
  while (responsible->isRunning() == false)
    responsible = responsible->getParent();
  return responsible;
}

bool
racehog::Thread::
isAncestorOf(const Thread* other) const {
  while (other != nullptr) {
    if (other == this)
      return true;
    other = other->getParent();
  }
  return false;
}

void
racehog::Thread::
acquireLock(LockId          id,
            const Metadata* loc) {
  auto search = locks.find(id);
  if (search != locks.end()) {
    (void) search->second.appendLocation(loc);
    return;
  }
  (void) locks.emplace(id, loc);
}

void
racehog::Thread::
releaseLock(LockId id) {
  (void) locks.erase(id);
}

void
racehog::Thread::
print(UserStream&        stream,
      const std::string& indent) const {
  stream << indent;
  (void) printId(stream);
  if (this != responsible) {
    stream << " -> ";
    (void) responsible->printId(stream);
  }
  if (is_running == true)
    stream << " (running)";
  else
    stream << " (stopped)";
}
