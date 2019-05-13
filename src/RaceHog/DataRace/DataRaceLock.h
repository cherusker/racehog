#ifndef RACEHOG_DATARACE_DATARACELOCK_H_
#define RACEHOG_DATARACE_DATARACELOCK_H_

#include <utility> // std::move

#include <boost/core/noncopyable.hpp>

#include "SteenX/Program/Metadata/Metadata.h"
#include "Support/turbovector.h"

namespace racehog {
  
  class DataRaceLock final
    : private boost::noncopyable {

  public:

    typedef turbovector<const Metadata*> Locations;

    DataRaceLock(const Metadata* loc)
      : locs(1) {
      (void) locs.push_back(loc);
    }

    DataRaceLock(const DataRaceLock& other)
      : locs(other.locs) {
      LOG(INFO) << "Copying DataRaceLock.";
    }

    DataRaceLock(DataRaceLock&& other)
      : locs(std::move(other.locs)) {
      LOG(INFO) << "Moving DataRaceLock.";
    }

    const Locations&
    getLocations(void) const {
      return locs;
    }

    void
    appendLocation(const Metadata* loc) {
      (void) locs.push_back(loc);
    }

  private:

    Locations locs;
  };
}

#endif // RACEHOG_DATARACE_DATARACELOCK_H_
