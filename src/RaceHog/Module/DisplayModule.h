#ifndef RACEHOG_MODULE_DISPLAYMODULE_H_
#define RACEHOG_MODULE_DISPLAYMODULE_H_

#include "RaceHog/Module/InputDataRaceQueueModule.h"
#include "RaceHog/Module/Module.h"

namespace racehog {

  /// Specification of Module for displays.

  class DisplayModule
    : public InputDataRaceQueueModule,
      public Module {

  };
}

#endif // RACEHOG_MODULE_DISPLAYMODULE_H_
