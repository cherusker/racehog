#ifndef RACEHOG_DATARACE_DATARACELOCKPAIR_H_
#define RACEHOG_DATARACE_DATARACELOCKPAIR_H_

#include <boost/core/noncopyable.hpp>

#include "RaceHog/DataRace/DataRaceLock.h"

namespace racehog {
  
  /// Holds two DataRaceLock objects to potentially prove a DataRace benign.

  class DataRaceLockPair final
    : private boost::noncopyable {

  public:

    DataRaceLockPair(const DataRaceLock& left,
                     const DataRaceLock& right)
      : left(left),
        right(right) {}

    const DataRaceLock&
    getLeftLock(void) const {
      return left;
    }

    const DataRaceLock&
    getRightLock(void) const {
      return right;
    }

  private:

    DataRaceLock left;
    DataRaceLock right;
  };
}

#endif // RACEHOG_DATARACE_DATARACELOCKPAIR_H_
