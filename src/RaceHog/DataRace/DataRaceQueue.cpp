#include "RaceHog/DataRace/DataRaceQueue.h"

void
racehog::DataRaceQueue::
push(DataRacePointer race) {
  std::unique_lock<std::mutex> lock(mutex);
  (void) races.push_back(std::move(race));
  (void) lock.unlock();
  (void) cond.notify_one();
}

racehog::DataRaceQueue::DataRacePointer
racehog::DataRaceQueue::
pop(void) {
  std::unique_lock<std::mutex> lock(mutex);
  while (is_alive == true && races.empty() == true)
    (void) cond.wait(lock);
  if (races.empty() == true)
    return nullptr; // Only reached when `is_alive == false`.
  auto race = std::move(races.front());
  (void) races.pop_front();
  return race;
}

void
racehog::DataRaceQueue::
shutdown(void) {
  std::unique_lock<std::mutex> lock(mutex);
  is_alive = false;
  (void) lock.unlock();
  (void) cond.notify_one();
}
