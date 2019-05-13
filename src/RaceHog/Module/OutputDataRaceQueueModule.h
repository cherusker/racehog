#ifndef RACEHOG_MODULE_OUTPUTDATARACEQUEUEMODULE_H_
#define RACEHOG_MODULE_OUTPUTDATARACEQUEUEMODULE_H_

#include "RaceHog/DataRace/DataRaceQueue.h"

namespace racehog {

  /// Part specificion for Module objects.

  class OutputDataRaceQueueModule {

  public:

    void
    setOutputDataRaceQueue(DataRaceQueue* out) {
      this->out = out;
    }

  protected:

    DataRaceQueue&
    getOutputDataRaceQueue(void) {
      return *out;
    }

  private:

    DataRaceQueue* out;
  };
}

#endif // RACEHOG_MODULE_OUTPUTDATARACEQUEUEMODULE_H_
