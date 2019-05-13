#include "Filters/DuplicateFilter/DuplicateFilter.h"

void
filters::DuplicateFilter::
run(void) {
  auto& in  = getInputDataRaceQueue();
  auto& out = getOutputDataRaceQueue();
  // TODO:  We need some serious hashing to get this check done in `O(1)`.
  while (auto race = in.pop()) {
    auto left   = race->getLeftAccess();
    auto right  = race->getRightAccess();
    bool is_dup = false;
    for (const auto& access : accesses) {
      if ((access.first == left  && access.second == right) ||
          (access.first == right && access.second == left)) {
        is_dup = true;
        break;
      }
    }
    if (is_dup)
      continue;
    (void) accesses.push_back(std::make_pair(left, right));
    (void) out.push(std::move(race));
  }
  (void) out.shutdown();
}
