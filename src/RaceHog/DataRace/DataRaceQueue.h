#ifndef RACEHOG_DATARACE_DATARACEQUEUE_H_
#define RACEHOG_DATARACE_DATARACEQUEUE_H_

#include <condition_variable>
#include <deque>
#include <memory> // unqiue_ptr
#include <mutex>

#include <boost/core/noncopyable.hpp>

#include "RaceHog/DataRace/DataRace.h"

namespace racehog {
  
  /// A concurrent, blocking queue for **one** producer and **one** consumer.

  class DataRaceQueue final
    : private boost::noncopyable {

  public:

    typedef std::unique_ptr<DataRace> DataRacePointer;

    /// Perfectly synchronised for **one** producer and **one** consumer.

    void
    push(DataRacePointer race);

    /// Blocks until it has a DataRace or returns `nullptr` on EOF.
    /**
     * It is the caller's responsibility to forward the DataRacePointer if
     * required.
     */

    DataRacePointer
    pop(void);

    /// Signals the end of this DataRaceQueue.
    /**
     * As a result, DataRaceQueue::popDataRace will return `nullptr` instead of
     * blocking, once it runs out of items.
     */

    void
    shutdown(void);

  private:

    typedef std::deque<DataRacePointer> DataRaces;

    DataRaces races;
    bool      is_alive = true;

    std::mutex              mutex;
    std::condition_variable cond;
  };
}

#endif // RACEHOG_DATARACE_DATARACEQUEUE_H_
