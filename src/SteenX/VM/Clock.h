#ifndef STEENX_VM_CLOCK_H_
#define STEENX_VM_CLOCK_H_

#include <cstdint>

#include <boost/core/noncopyable.hpp>

namespace racehog {

  /// Simple Lamport's clock.

  class Clock
    : private boost::noncopyable {

  public:
    
    typedef uint64_t Time; ///< Arbitrarily large, non-negative format.

    /// Return the internal time and _then_ increase it.

    Time
    tick(void) {
      return time++;
    }

  private:

    Time time = 0;
  };
}

#endif // STEENX_VM_CLOCK_H_
