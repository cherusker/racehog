#ifndef DISPLAYS_COUNTINGDISPLAY_COUNTINGDISPLAY_H_
#define DISPLAYS_COUNTINGDISPLAY_COUNTINGDISPLAY_H_

#include "RaceHog/Module/DisplayModule.h"
#include "Support/Stream/UserStream.h"

namespace displays {

  /// Counts and discards DataRace objects.

  class CountingDisplay final
    : public racehog::DisplayModule {

  public:

    CountingDisplay(racehog::UserStream& stream)
      : stream(stream) {}

    void
    run(void) override;

  private:

    racehog::UserStream& stream;
  };
}

#endif // DISPLAYS_COUNTINGDISPLAY_COUNTINGDISPLAY_H_
