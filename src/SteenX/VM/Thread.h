#ifndef STEENX_VM_THREAD_H_
#define STEENX_VM_THREAD_H_

#include <unordered_map>

#include <boost/core/noncopyable.hpp>

#include "RaceHog/DataRace/DataRaceLock.h"
#include "SteenX/Program/Metadata/Metadata.h"
#include "Support/Stream/Printable.h"
#include "Support/Stream/Stream.h"
#include "Support/turbovector.h"

namespace racehog {

  /// A thread in the serialised execution of Program.

  class Thread final
    : public  Printable,
      private boost::noncopyable {

  public:

    /// Simple `unsigned` id since Thread is not streamed binarily.

    typedef unsigned Id;
    typedef unsigned LockId;

    typedef std::unordered_map<LockId, DataRaceLock> Locks;

    /// Return `nullptr` if it is the main thread of a process.

    Thread(Id id)
      : id(id) {}

    Id
    getId(void) const {
      return id;
    }

    Thread*
    getParent(void) {
      return parent;
    }

    /// Return `nullptr` if it is the main thread of a process.

    const Thread*
    getParent(void) const {
      return parent;
    }

    /**
     * Set \p thread to `nullptr` if it has no parents.
     * This is only the case for the _one_ main thread of a process however, and
     * should never be set explicitely.
     */

    void
    setParent(Thread* thread) {
      parent = thread;
    }

    void
    start(void) {
      is_running = true;
    }

    void
    stop(void) {
      is_running = false;
    }

    /// Return `true` between Thread::start and Thread::stop.
    /**
     * Thread starts _not_ running; hence `Thread::isRunning == false`.
     */

    bool
    isRunning(void) const {
      return is_running;
    }

    /// Thread that is responsible of this Thread's actions.
    /**
     * This describes the Thread itself, if it is running.
     * If the current thread has been joind already, the "topmost" running
     * parent Thread is held responsible.
     *
     * \return `nullptr` if no responsible Thread can be found.
     *         In well formed programs this only occurs after joining the
     *         process's main thread (thus never ... generally).
     *
     * Cannot be `const`because it buffers the return value internally for
     * faster lookups.
     */

    Thread*
    getResponsibleThread(void);

    /// Determine if `this` is an ancestor of \p other.
    /**
     * _Ancestor_ is defined as
     * `this == other[->getParent()[->getParent()...]]`.
     * This means that Thread::isAncestorOf also returns `true` for
     * `this == other`.
     */

    bool
    isAncestorOf(const Thread* other) const;

    void
    acquireLock(LockId          id,
                const Metadata* loc);

    void
    releaseLock(LockId id);

    const Locks&
    getLocks(void) const {
      return locks;
    }

    /// `TX` where `X` is Thread::getId.

    void
    printId(UserStream&        stream,
            const std::string& indent = "") const {
      stream << indent
             << stream.setFormat(UserStream::Format::Variable)
             << "T" << getId()
             << stream.resetFormat();
    }

    /// Thread::printId plus responsible thread, and state (Thread::isRunning).

    void
    print(UserStream&        stream,
          const std::string& indent = "") const override;

  private:

    Id      id;
    Thread* parent      = nullptr;
    Thread* responsible = this; ///< Buffer to keep lookups in O(1).
    bool    is_running  = false;

    Locks locks;
  };
}

#endif // STEENX_VM_THREAD_H_
