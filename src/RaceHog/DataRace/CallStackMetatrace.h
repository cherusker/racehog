#ifndef RACEHOG_DATARACE_CALLSTACKMETATRACE_H_
#define RACEHOG_DATARACE_CALLSTACKMETATRACE_H_

#include <boost/core/noncopyable.hpp>

#include "RaceHog/DataRace/CallStackMetaframe.h"
#include "Support/Logging.h"
#include "Support/range_iterator.h"
#include "Support/Stream/Printable.h"
#include "Support/turbovector.h"

namespace racehog {
  
  /// Polished \ref turbovector of CallStackMetaframe objects.

  class CallStackMetatrace final
    : public  Printable,
      private boost::noncopyable {

  public:

    typedef turbovector<CallStackMetaframe> CallStackMetaframes;
    typedef
      range_iterator<CallStackMetaframes::const_iterator>
      ConstCallStackMetaframesRange;

    CallStackMetatrace(void)
      : frames(num_init_frames) {}

    CallStackMetatrace(const CallStackMetatrace& other)
      : frames(other.frames) {
      LOG(INFO) << "Copying CallStackMetatrace.";
    }

    CallStackMetatrace(CallStackMetatrace&& other)
      : frames(std::move(other.frames)) {
      LOG(INFO) << "Moving CallStackMetatrace.";
    }

    bool
    operator==(const CallStackMetatrace& other) const {
      return frames == other.frames ? true : false;
    }

    CallStackMetaframes::size_type
    getNumFrames(void) const {
      return frames.size();
    }

    ConstCallStackMetaframesRange
    getFrames(void) const {
      return ConstCallStackMetaframesRange(frames);
    }

    /// Push a new CallStackMetaframe to the internal stack of frames.
    /**
     * CallStackMetaframe objects are expected to be pushed with NamedMetadata
     * that belongs to the _called_ function (no the callee).
     * This NamedMetadata sticks around until popped via
     * CallStackMetatrace::popFrame and can not be altered.
     */

    void
    pushFrame(const NamedMetadata* func) {
      (void) frames.push_back(CallStackMetaframe(func, func));
    }

    void
    popFrame(void) {
      (void) frames.pop_back();
    }

    /// Update the location of the _most recent_ CallStackMetaframe.
    /**
     * The behaviour is undefined if CallStackMetatrace does not have any
     * CallStackMetaframe objects at the time of calling
     * CallStackMetatrace::updateLocation.
     * In fact, things will most certainly go up in flames.
     */

    void
    updateLocation(const Metadata* loc) {
      (void) frames.back().setLocation(loc);
    }

    void
    print(UserStream&        stream,
          const std::string& indent = "") const override;

  private:

    // TODO:  Discuss this number thoroughly.
    static constexpr CallStackMetaframes::size_type num_init_frames = 16;

    CallStackMetaframes frames;
  };
}

#endif // RACEHOG_DATARACE_CALLSTACKMETATRACE_H_
