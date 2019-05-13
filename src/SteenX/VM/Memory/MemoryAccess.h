#ifndef STEENX_VM_MEMORY_MEMORYACCESS_H_
#define STEENX_VM_MEMORY_MEMORYACCESS_H_

#include <boost/core/noncopyable.hpp>

#include "RaceHog/DataRace/DataRaceAccess.h"
#include "SteenX/VM/Clock.h"
#include "SteenX/VM/Thread.h"
#include "Support/Logging.h"

namespace racehog {
  
  class MemoryAccess final
    : public DataRaceAccess {

  public:

    MemoryAccess(Mode                      mode,
                 Clock::Time               time,
                 Thread*                   thread,
                 const CallStackMetatrace& trace)
      : DataRaceAccess(mode, thread->getId(), trace),
        time(time),
        thread(thread),
        locks(thread->getLocks()) {}

    Clock::Time
    getTime(void) const {
      return time;
    }

    const Thread::Locks&
    getLocks(void) const {
      return locks;
    }

    /// Not `const` since it calls Thread::getResponsibleThread.

    bool
    racesWith(MemoryAccess& other);

    /// Puts `R` (read), `W` (write) or `X` (invalid) on the stream.
    /**
     * In addition, it also adds the used thread.
     */

    void
    print(UserStream&        stream,
          const std::string& indent = "") const override;

  private:

    Clock::Time   time;
    Thread*       thread;
    Thread::Locks locks;
  };
}

#endif // STEENX_VM_MEMORY_MEMORYACCESS_H_
