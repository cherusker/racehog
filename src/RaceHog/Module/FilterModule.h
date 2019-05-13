#ifndef RACEHOG_MODULE_FILTERMODULE_H_
#define RACEHOG_MODULE_FILTERMODULE_H_

#include "RaceHog/Module/InputDataRaceQueueModule.h"
#include "RaceHog/Module/Module.h"
#include "RaceHog/Module/OutputDataRaceQueueModule.h"

namespace racehog {

  /// Specification of Module for filters.

  class FilterModule
    : public InputDataRaceQueueModule,
      public Module,
      public OutputDataRaceQueueModule {

  };
}

#endif // RACEHOG_MODULE_FILTERMODULE_H_
