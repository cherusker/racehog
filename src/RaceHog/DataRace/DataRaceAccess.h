#ifndef RACEHOG_DATARACE_DATARACEACCESS_H_
#define RACEHOG_DATARACE_DATARACEACCESS_H_

#include <utility> // std::move

#include <boost/core/noncopyable.hpp>

#include "RaceHog/DataRace/CallStackMetatrace.h"
#include "Support/Logging.h"
#include "Support/range_iterator.h"
#include "Support/Stream/Printable.h"
#include "Support/Stream/Stream.h"

namespace racehog {
  
  /// An access that forms a DataRace together with another access.
  /**
   * Cannot be `final` as MemoryAccess derives from DataRaceAccess.
   */

  class DataRaceAccess
    : public  Printable,
      private boost::noncopyable {

  public:

    typedef Stream::AccessMode Mode;
    typedef Stream::ThreadId   ThreadId;

    DataRaceAccess(Mode                      mode,
                   ThreadId                  tid,
                   const CallStackMetatrace& trace)
      : mode(mode),
        tid(tid),
        trace(trace) {}

    DataRaceAccess(const DataRaceAccess& other)
      : mode(other.mode),
        tid(other.tid),
        trace(other.trace) {
      LOG(INFO) << "Copying DataRaceAcccess.";
    }

    DataRaceAccess(DataRaceAccess&& other)
      : mode(other.mode),
        tid(other.tid),
        trace(std::move(other.trace)) {
      LOG(INFO) << "Moving DataRaceAcccess.";
    }

    /// Compares DataRaceAccess::getMode and DataRaceAccess::getThreadId first.
    /**
     * DataRaceAccess::getCallStackMetatrace only gets compared when the two
     * attributes match.
     * That should speed up things considerably for some instances.
     */

    bool
    operator==(const DataRaceAccess& other) const {
      return mode  == other.mode  &&
             tid   == other.tid   &&
             trace == other.trace ? true : false;
    }

    Mode
    getMode(void) const {
      return mode;
    }

    ThreadId
    getThreadId(void) const {
      return tid;
    }

    const CallStackMetatrace&
    getCallStackMetatrace(void) const {
      return trace;
    }

    void
    print(UserStream&        stream,
          const std::string& indent = "") const override;

  private:

    Mode               mode = Mode::Invalid;
    ThreadId           tid  = 0;
    CallStackMetatrace trace;
  };
}

#endif // RACEHOG_DATARACE_DATARACEACCESS_H_
