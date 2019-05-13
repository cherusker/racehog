#ifndef RACEHOG_MODULE_MODULE_H_
#define RACEHOG_MODULE_MODULE_H_

#include <boost/core/noncopyable.hpp>

namespace racehog {

  /// Base class for all module of RaceHog.

  class Module
    : private boost::noncopyable {

  public:

    /// Start the execution of a specific Module.

    virtual void
    run(void) = 0;
  };
}

#endif // RACEHOG_MODULE_MODULE_H_
