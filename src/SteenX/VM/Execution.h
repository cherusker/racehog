#ifndef STEENX_VM_EXECUTION_H_
#define STEENX_VM_EXECUTION_H_

#include <boost/core/noncopyable.hpp>

#include "RaceHog/DataRace/DataRaceQueue.h"
#include "SteenX/Program/Instruction/InstructionVisitor.h"
#include "SteenX/Program/Operand/Operand.h"
#include "SteenX/Program/Program.h"
#include "SteenX/VM/Environment.h"
#include "SteenX/VM/Equaliser.h"
#include "SteenX/VM/Memory/MemoryAccess.h"
#include "Support/Stream/Stream.h"
#include "Support/Stream/UserStream.h"

namespace racehog {
  
  /// _One_ deterministic execution thread through a Program.

  class Execution final
    : public  InstructionVisitor<Execution, bool>,
      private boost::noncopyable {

  public:

    /// Constructs an Execution that is ready for work.
    /**
     *   - All subsystems (like MemoryStack) will be initialised.
     *   - The `main` thread will be initialised.
     *
     * \param stream Means of user output and synchronisation.
     * \param races  Storage for found DataRace objects.
     * \param prog   The Program to be executed.
     */

    Execution(UserStream&    stream,
              DataRaceQueue& races,
              const Program& prog);

    /// Execute a Program.
    /**
     * Every entry point will be split into its own Execution with its own
     * CallContext.
     * Then, every Execution will start working on its instructions and start
     * collecting DataRace objects.
     *
     * \return `true` if the execution was successful, `false` otherwise.
     *         Note that the return value is independent of whether data races
     *         have been found or not;  DataRaceQueue has to be questioned
     *         for that.
     *         The return value `false` hints at serious issues with the
     *         executed Program that led to an early stop in the execution.
     *         In any case, some (descriptive) output has landed on
     *         UserStream::fatal when Execution::run returns `false` so there is
     *         no need to for further information for users;  just abort
     *         gracefully as soon as possible.
     */

    bool
    run(void);

    /// Basically Execution::run, with lots of printing and pausing.
    /**
     * Furthermore, _no_ parallel execution at any time.
     */

    bool
    interactiveRun(void);

    /// Fallback;  will terminate with an error.

    bool
    visitInstruction(const Instruction& inst) {
      RH_UNUSED(inst);
      RH_FATAL_FALSE_BROKEN_PROGRAM("Unexpected instruction.");
    }

    bool
    visitForkInstruction(const ForkInstruction& inst);

    bool
    visitJoinInstruction(const JoinInstruction& inst);

    bool
    visitAcquireInstruction(const AcquireInstruction& inst);
    
    bool
    visitReleaseInstruction(const ReleaseInstruction& inst);

    bool
    visitCallInstruction(const CallInstruction& inst);

    bool
    visitReturnInstruction(const ReturnInstruction& inst);

    bool
    visitReferenceInstruction(const ReferenceInstruction& inst);

    bool
    visitEqualiseInstruction(const EqualiseInstruction& inst);

    bool
    visitReadInstruction(const ReadInstruction& inst) {
      return visitAccessInstructionImpl(inst, MemoryAccess::Mode::Read);
    }

    bool
    visitWriteInstruction(const WriteInstruction& inst) {
      return visitAccessInstructionImpl(inst, MemoryAccess::Mode::Write);
    }

    bool
    visitAllocateInstruction(const AllocateInstruction& inst);

  private:

    typedef Stream::MemoryAtomPointerOffset PointerOffset;

    UserStream&    stream;
    DataRaceQueue& races;
    Environment    env;
    Equaliser      eq;

    /// Return value of this Execution.
    
    MemoryAtom exe_retval;

    /// Helper/workaround for Execution::beginFunctionCall.
    /**
     * Extracts Function or `nullptr` from Operand.
     */

    const Function*
    extractFunction(const Operand& op);

    bool
    beginFunctionCall(const CallInstructionBase& inst,
                      MemoryAtom*                retval);

    /// Let \p val point to \p base under offset + range of \p inst.

    void
    resolveReference(const ReferenceInstruction& inst,
                     MemoryAtom*               val,
                     MemoryAtom*               base);

    bool
    visitAccessInstructionImpl(const ValueInstructionBase& inst,
                               MemoryAccess::Mode          mode);
  };
}

#endif // STEENX_VM_EXECUTION_H_
