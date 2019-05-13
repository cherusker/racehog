#ifndef FILTERS_DUPLICATEFILTER_DUPLICATEFILTER_H_
#define FILTERS_DUPLICATEFILTER_DUPLICATEFILTER_H_

#include <deque>
#include <utility> // pair

#include "RaceHog/Module/FilterModule.h"

namespace filters {

  /// Duplicate detection based on accesses of DataRace objects.

  class DuplicateFilter final
    : public racehog::FilterModule {

    void
    run(void) override;

  private:

    typedef std::deque<std::pair<
                         racehog::DataRaceAccess,
                         racehog::DataRaceAccess>> Accesses;

    Accesses accesses;
  };
}

#endif // FILTERS_DUPLICATEFILTER_DUPLICATEFILTER_H_
