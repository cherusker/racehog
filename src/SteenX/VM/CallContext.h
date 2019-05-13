#ifndef STEENX_VM_CALLCONTEXT_H_
#define STEENX_VM_CALLCONTEXT_H_

#include <boost/core/noncopyable.hpp>

#include "SteenX/Program/Function.h"
#include "SteenX/Program/Instruction/Instruction.h"
#include "SteenX/VM/Memory/MemoryAtom.h"
#include "Support/Logging.h"

namespace racehog {
  
  /// A classic stack frame, enhanced by dynamic context awareness.
  /**
   * Apart from the the _usual_ stack frame duties (knowing about the callee
   * and local stack memory) CallContext is also aware of the next Instruction
   * (it basically holds the instruction pointer).
   */

  class CallContext final
    : private boost::noncopyable {

  public:

    // TODO:  What about retval == void (void return value)?

    /**
     * \param local_base_id Upon calling the constructor, `local_base_id` has to
     *        point to a valid MemoryAtom within MemoryStack as accessing and
     *        caching could start immediately.
     *        Ergo:  resize MemoryStack first and create CallContext afterwards.
     */

    CallContext(MemoryAtom::Id  local_base_id,
                MemoryAtom*     retval,
                const Function& func)
      : local_base_id(local_base_id),
        retval(retval),
        func(func),
        num_insts(func.getNumInstructions()) {}

    const Function&
    getFunction(void) const {
      return func;
    }

    /// Tandem with CallContext::getNextInstruction.
    /**
     * Tests if there is another instruction to request via
     * CallContext::hasNextInstruction.
     */

    bool
    hasNextInstruction(void) const {
      return inst_id >= num_insts ? false : true;
    }

    /// Tandem with CallContext::hasNextInstruction.
    /**
     * Returns the _next_ instruction of the CallContext.  This function is to
     * be understood as an iterator over all Instruction objects of the current
     * CallContext; every call will return _the next_ (another) Instruction
     * (CallContext::getNextInstruction `!=` CallContext::getNextInstruction).
     *
     * The return value is undefined if called when
     * CallContext::hasNextInstruction returns false.
     */

    const Instruction&
    getNextInstruction(void) {
      return func.getInstruction(inst_id++);
    }

    /// Id of first MemoryAtom on MemoryStack that belongs to the CallContext.

    MemoryAtom::Id
    getLocalBaseId(void) const {
      return local_base_id;
    }

    MemoryAtom*
    getReturnValue(void) {
      return retval;
    }

    const MemoryAtom*
    getReturnValue(void) const {
      return retval;
    }

    void
    setReturnValue(MemoryAtom* retval) {
      this->retval = retval;
    }

    /// Default: `false`.

    bool
    isThreadBase(void) const {
      return is_thread_base;
    }

    void
    setIsThreadBase(bool is_base = true) {
      is_thread_base = is_base;
    }

  private:

    MemoryAtom::Id local_base_id;
    MemoryAtom*    retval;

    const Function& func;

    Instruction::Id inst_id = 0;
    Instruction::Id num_insts;

    bool is_thread_base = false;
  };
}

#endif // STEENX_VM_CALLCONTEXT_H_
