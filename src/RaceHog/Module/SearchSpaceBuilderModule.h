#ifndef RACEHOG_MODULE_SEARCHSPACEBUILDERMODULE_H_
#define RACEHOG_MODULE_SEARCHSPACEBUILDERMODULE_H_

#include "RaceHog/Module/Module.h"
#include "RaceHog/Module/OutputDataRaceQueueModule.h"

namespace racehog {

  /// Specification of Module for filters.

  class SearchSpaceBuilderModule
    : public Module,
      public OutputDataRaceQueueModule {

  };
}

#endif // RACEHOG_MODULE_SEARCHSPACEBUILDERMODULE_H_
