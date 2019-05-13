#include "Displays/ConsoleDisplay/ConsoleDisplay.h"

void
displays::ConsoleDisplay::
run(void) {
  auto& in = getInputDataRaceQueue();
  stream << "Data race candidates:\n";
  size_t num_races = 0;
  while (auto race = in.pop()) {
    stream << stream.setForeground(racehog::UserStream::Colour::Green)
           << "  Data race " << ++num_races
           << stream.resetForeground()
           << ":\n";
    (void) race->print(stream, "    ");
    stream << "\n";
  }
  if (num_races == 0)
    stream << "  none\n";
}
