#ifndef DISPLAYS_CONSOLEDISPLAY_CONSOLEDISPLAY_H_
#define DISPLAYS_CONSOLEDISPLAY_CONSOLEDISPLAY_H_

#include "RaceHog/Module/DisplayModule.h"
#include "Support/Stream/UserStream.h"

namespace displays {

  /// Put DataRace objects to the console in rudimentary fashion.

  class ConsoleDisplay final
    : public racehog::DisplayModule {

  public:

    ConsoleDisplay(racehog::UserStream& stream)
      : stream(stream) {}

    void
    run(void) override;

  private:

    racehog::UserStream& stream;
  };
}

#endif // DISPLAYS_CONSOLEDISPLAY_CONSOLEDISPLAY_H_
