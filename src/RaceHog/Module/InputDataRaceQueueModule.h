#ifndef RACEHOG_MODULE_INPUTDATARACEQUEUEMODULE_H_
#define RACEHOG_MODULE_INPUTDATARACEQUEUEMODULE_H_

#include "RaceHog/DataRace/DataRaceQueue.h"

namespace racehog {

  /// Part specificion for Module objects.

  class InputDataRaceQueueModule {

  public:

    void
    setInputDataRaceQueue(DataRaceQueue* in) {
      this->in = in;
    }

  protected:

    DataRaceQueue&
    getInputDataRaceQueue(void) {
      return *in;
    }

  private:

    DataRaceQueue* in;
  };
}

#endif // RACEHOG_MODULE_INPUTDATARACEQUEUEMODULE_H_
