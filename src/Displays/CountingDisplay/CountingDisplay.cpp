#include "Displays/CountingDisplay/CountingDisplay.h"

void
displays::CountingDisplay::
run(void) {
  size_t num_races = 0;
  auto& in = getInputDataRaceQueue();
  while (auto race = in.pop())
    ++num_races;
  stream << num_races << "\n";
}
