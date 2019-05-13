#ifndef RACEHOG_DATARACE_DATARACEPROTECTION_H_
#define RACEHOG_DATARACE_DATARACEPROTECTION_H_

#include <boost/core/noncopyable.hpp>

#include "Program/Metadata/Metadata.h"
#include "Support/turbovector.h"

namespace racehog {
  
  /// Holds Metadata objects that point to mutex acquiring locations.

  class DataRaceProtection final
    : private boost::noncopyable {

  public:

    typedef turbovector<const Metadata*> Locations;

    const Locations&
    getLeft(void) const {
      return l;
    }

    const Locations&
    getRight(void) const {
      return r;
    }

  private:

    Locations l;
    Locations r;
  };
}

#endif // RACEHOG_DATARACE_DATARACEPROTECTION_H_
