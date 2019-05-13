#ifndef STEENX_VM_ENVIRONMENT_H_
#define STEENX_VM_ENVIRONMENT_H_

#include <deque>

#include <boost/core/noncopyable.hpp>

#include "RaceHog/DataRace/CallStackMetatrace.h"
#include "SteenX/Program/Function.h"
#include "SteenX/Program/Operand/Operand.h"
#include "SteenX/Program/Program.h"
#include "SteenX/VM/CallContext.h"
#include "SteenX/VM/Clock.h"
#include "SteenX/VM/Memory/MemoryAtom.h"
#include "SteenX/VM/Memory/MemoryStore.h"
#include "SteenX/VM/Thread.h"
#include "Support/Logging.h"
#include "Support/turbovector.h"

namespace racehog {
  
  /// Manages runtime data like ThreadPool, MemoryStack, CallContext, ...
  /**
   * Environment is designed as a pure interface (with a few convencience
   * functions) to all underlaying sytems.
   *
   * Environment should know/care as little as possible about the _meaning_ of
   * the data it holds as well as the connection between different subsystems.
   */

  class Environment final
    : private boost::noncopyable {

  public:

    /// Persistent pool of Thread objects that will be stored forever.

    typedef std::deque<Thread> Threads;

    /**
     * In addition to Threads, we need a data structure that tracks the
     * currently evaluated Thread (in stack-like form).
     * As this behaviour is closer to actual Execution logic than to simply
     * storing Thread objects, many functions in Environment deal with that
     * (instead of with Threads).
     */

    typedef turbovector<Thread*> ThreadStack;

    typedef std::deque<MemoryStore> MemoryStores;

    Environment(const Program& prog);

    Clock&
    getClock(void) {
      return clock;
    }

    const Threads&
    getThreads(void) const {
      return threads;
    }

    const ThreadStack&
    getThreadStack(void) const {
      return tstack;
    }

    /// Push a _new_ Thread onto the internal Thread stack.
    /**
     * Sets the parent of a thread (Thread::setParent) but does _not_ start
     * the thread;  Thread::start has to be called manually.
     */

    Thread*
    pushThread(void);

    /// Pop the last Thread from the internal Thread stack.
    /**
     * Popped Thread objects will stay alive forever as their accesses and
     * return values could be used at a later point in the program.
     */

    void
    popThread(void) {
      (void) tstack.pop_back();
    }

    /// At least one thread on the internal Thread stack.

    bool
    hasActiveThread(void) {
      return tstack.empty() == false;
    }

    /// No bounds check;  requires Environment::hasActiveThread `== true`.
    /**
     * Returns the last Thread that was added by Environment::beginThread minus
     * all Thread objects that were popped by Environment::endThread.
     *
     * Validity (Environment::hasActiveThread) has to be checked _upfront_.
     */

    Thread*
    getActiveThread(void) {
      return tstack.back();
    }

    /// Get the return value MemoryAtom of \p thread.
    /**
     * All threads used in Environment::getThreadReturnValue have to be created
     * through Environment::pushThread.
     * Otherwise the behaviour is undefined and will likely lead to crashes.
     * 
     * The main reason for storing return values far away from Thread objects is
     * to avoid cyclic references between MemoryAtom and Thread.
     *
     * Also (but this is really just a side effect), return values play only a
     * minor role in thread management which avoids loading pointers to MemoryAtom
     * objectst into the cache frequently without using them.
     */

    MemoryAtom*
    getThreadReturnValue(const Thread* thread) {
      return (*tretvals)[thread->getId()];
    }

    /// Returns a new (unique) lock id.
    /**
     * Lock ids start at `1` to reserve `0` as special value.
     */

    Thread::LockId
    getNextLockId(void) {
      return ++max_lock_id;
    }

    const MemoryStores&
    getMemoryStores(void) const {
      return mstores;
    }

    MemoryStore&
    pushMemoryStore(MemoryAtom::Id num_atoms) {
      (void) mstores.emplace_back(mstores.size(), num_atoms);
      return mstores.back();
    }

    /// Returns `nullptr` when facing illegal \p id.
    /**
     * _Raw_ memory atom lookup in the first MemoryStore which is used as
     * common program stack.
     * Useful when offsetting known pointers.
     */

    MemoryAtom*
    getMemoryAtom(MemoryAtom::Id id) {
      return (*mstack)[id];
    }

    /// See Environment::getMemoryAtom(MemoryAtom::Id).

    const MemoryAtom*
    getMemoryAtom(MemoryAtom::Id id) const {
      return (*mstack)[id];
    }

    /// Returns `nullptr` when facing illegal \p id.
    /**
     * _Raw_ memory atom lookup in \p sid MemoryStore.
     * Useful when offsetting known pointers.
     */

    MemoryAtom*
    getMemoryAtom(MemoryStore::Id sid,
                  MemoryAtom::Id  id) {
      // TODO:  `sid` bounds check?
      return mstores[sid][id];
    }

    /// See Environment::getMemoryAtom(MemoryStore::Id, MemoryAtom::Id).

    const MemoryAtom*
    getMemoryAtom(MemoryStore::Id sid,
                  MemoryAtom::Id  id) const {
      // TODO:  `sid` bounds check?
      return mstores[sid][id];
    }

    /// Returns `nullptr` when facing illegal Operand.
    /**
     * Looks up the MemoryAtom, specified by Operand.
     * Local lookups will always happen on the topmost CallContext.
     */

    MemoryAtom*
    getMemoryAtom(const Operand& op) {
      return getMemoryAtom(getActiveCallContext(), op);
    }

    /// See Environment::getMemoryAtom(const Operand&).
    
    const MemoryAtom*
    getMemoryAtom(const Operand& op) const {
      return getMemoryAtom(getActiveCallContext(), op);
    }

    /**
     * Same as Environment::getMemoryAtom(const Operand&) except that local
     * lookups will happen on the given CallContext.
     */

    MemoryAtom*
    getMemoryAtom(CallContext&   context,
                  const Operand& op);

    /// See Environment::getMemoryAtom(CallContext&, const Operand&).

    const MemoryAtom*
    getMemoryAtom(const CallContext& context,
                  const Operand&     op) const;

    /// Arrange everything that's necessary for a successful function call.
    /**
     *   - Create a new CallContext.
     *   - Add local MemoryAtom objects to MemoryStack.
     *   - Add a CallStackMetaframe to the internal CallStackMetatrace.
     *
     * \param func Cannot be `nullptr`.
     * \param retval MemoryAtom within callee.
     * \return The new CallContext.
     */

    CallContext&
    pushCallContext(const Function& func,
                    MemoryAtom*     retval);

    /// Cleanup after a function return (called implicitely).
    /**
     * Note:  Environment::popCallContext is implicitely called by
     * Environment::getNextInstruction.
     *
     *   - Pop the topmost CallContext.
     *   - Call Environment::popThread if CallContext::isThreadBase.
     *   - Remove local MemoryAtom objects from MemoryStack.
     *   - Pop topmost CallStackMetaframe from internal CallStackMetatrace.
     */

    void
    popCallContext(void);

    bool
    hasActiveCallContext(void) const {
      return contexts.empty() == true ? false : true;
    }

    /// Returns the most current CallContext ("top of call stack").

    CallContext&
    getActiveCallContext(void) {
      return contexts.back();
    }

    /// See Environment::getActiveCallContext.

    const CallContext&
    getActiveCallContext(void) const {
      return contexts.back();
    }

    /// Tandem with Environment::getNextInstruction.
    /**
     * Tests if there is at least one active CallContext on the stack that has
     * at least one Instruction left that should be executed
     * (CallContext::hasNextInstruction).
     */

    bool
    hasNextInstruction(void) const;

    /// Tandem with Environment::hasNextInstruction.
    /**
     * Uses CallContext::getNextInstruction and cleans up finished CallContext
     * objects from the internal call stack.
     *
     * The return value is undefined if called when
     * Environment::hasNextInstruction returns false.
     */

    const Instruction&
    getNextInstruction(void);

    /// CallStackMetatrace kept up to day through hooking into:
    /**
     *   - Environment::pushCallContext
     *   - Environment::popCallContext
     *   - Environment::getNextInstruction
     */

    const CallStackMetatrace&
    getCallStackMetatrace(void) const {
      return trace;
    }

  private:

    /// `std::deque` is the obvious choice here.
    
    typedef std::deque<CallContext> CallContexts; 

    /// Initialisation size of ThreadStack and Environment::tretvals.

    static constexpr ThreadStack::size_type num_init_threads = 16;

    Clock        clock;
    Threads      threads;
    ThreadStack  tstack;   // TODO:  shrink_to_fit
    MemoryStores mstores;
    MemoryStore* mstack;   ///< `&mstores[0]` (memory stack).
    MemoryStore* tretvals; ///< `&mstores[1]` (thread return values).
    CallContexts contexts; // TODO:  shrink_to_fit

    /**
     * The idea of keeping CallStackMetatrace separately from CallContexts is
     * to make it trivial to copy CallStackMetatrace.
     * We need to fully copy CallStackMetatrace within every ReadInstruction and
     * every WriteInstruction that we encounter, sometimes even several times
     * within one of these instructions.
     * Traversing CallContexts and building CallStackMetatrace from base within
     * every instruction would be very expensive.
     */

    CallStackMetatrace trace;

    Thread::LockId max_lock_id = 0;
  };
}

#endif // STEENX_VM_ENVIRONMENT_H_
