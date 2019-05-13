#ifndef RACEHOG_DATARACE_DATARACE_H_
#define RACEHOG_DATARACE_DATARACE_H_

#include <deque>
#include <string>
#include <utility> // std::move

#include <boost/core/noncopyable.hpp>

#include "Support/Stream/Printable.h"
#include "RaceHog/DataRace/DataRaceAccess.h"
#include "RaceHog/DataRace/DataRaceLock.h"
#include "RaceHog/DataRace/DataRaceLockPair.h"

namespace racehog {
  
  /// Holds two DataRaceAccess that potentially form a data race.

  class DataRace final
    : public  Printable,
      private boost::noncopyable {

  public:

    typedef std::deque<DataRaceLockPair> LockPairs;

    DataRace(const DataRaceAccess& left,
             const DataRaceAccess& right)
      : left(left),
        right(right) {}

    const DataRaceAccess&
    getLeftAccess(void) const {
      return left;
    }

    const DataRaceAccess&
    getRightAccess(void) const {
      return right;
    }

    const LockPairs&
    getLockPairs(void) const {
      return locks;
    }

    /// Emplaces a new DataRaceLockPair the internal list of pairs.
    /**
     * The emplacement is unconditional;  no (sanity) checks will be carried
     * out.
     */

    void
    appendLockPair(const DataRaceLock& left,
                   const DataRaceLock& right) {
      (void) locks.emplace_back(left, right);
    }

    void
    print(UserStream&        stream,
          const std::string& indent = "") const override;

  private:

    DataRaceAccess left;
    DataRaceAccess right;
    LockPairs      locks;

    void
    printLock(UserStream&           stream,
              const std::string&    indent,
              const DataRaceAccess& access,
              const DataRaceLock&   lock) const;
  };
}

#endif // RACEHOG_DATARACE_DATARACE_H_
