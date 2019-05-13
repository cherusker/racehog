#ifndef RACEHOG_DATARACE_CALLSTACKMETAFRAME_H_
#define RACEHOG_DATARACE_CALLSTACKMETAFRAME_H_

#include <boost/core/noncopyable.hpp>

// TODO:  Don't have CallStackMetaframe dependend on SteenX!

#include "SteenX/Program/Metadata/Metadata.h"
#include "SteenX/Program/Metadata/NamedMetadata.h"
#include "Support/Logging.h"

namespace racehog {
  
  /// Holds Metadata that describes a stack frame at a specific point in time.

  class CallStackMetaframe final
    : private boost::noncopyable {

  public:

    CallStackMetaframe(const NamedMetadata* func = nullptr,
                       const Metadata*      loc  = nullptr)
      : func(func),
        loc(loc) {}

    CallStackMetaframe(const CallStackMetaframe& other)
      : func(other.func),
        loc(other.loc) {
      LOG(INFO) << "Copying CallStackMetaframe.";
    }

    CallStackMetaframe(CallStackMetaframe&& other)
      : func(other.func),
        loc(other.loc) {
      LOG(INFO) << "Moving CallStackMetaframe.";
    }

    /// Mind the `nullptr`.

    const NamedMetadata*
    getFunction(void) const {
      return func;
    }

    void
    setFunction(const NamedMetadata* func) {
      this->func = func;
    }

    /// Mind the `nullptr`.

    const Metadata*
    getLocation(void) const {
      return loc;
    }

    void
    setLocation(const Metadata* loc) {
      this->loc = loc;
    }

  private:

    const NamedMetadata* func;
    const Metadata*      loc;
  };
}

#endif // RACEHOG_DATARACE_CALLSTACKMETAFRAME_H_
