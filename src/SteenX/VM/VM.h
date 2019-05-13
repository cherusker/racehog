#ifndef STEENX_VM_VM_H_
#define STEENX_VM_VM_H_

#include <boost/core/noncopyable.hpp>

#include "SteenX/Program/Program.h"
#include "SteenX/Program/ProgramSyntaxChecker.h"
#include "RaceHog/DataRace/DataRaceQueue.h"
#include "Support/Stream/UserStream.h"

namespace racehog {

  /// The virtual machine.

  class VM final
    : private boost::noncopyable {

  public:
    
    VM(UserStream& stream)
      : stream(stream),
        checker(stream) {}

    /// ProgramSyntaxChecker::verify and  Execution::run.

    bool
    run(const Program& prog,
        DataRaceQueue& races);

    /// ProgramSyntaxChecker::verify and Execution::interactiveRun.

    bool
    interactiveRun(const Program& prog,
                   DataRaceQueue& races);

  private:

    UserStream&          stream;
    ProgramSyntaxChecker checker;
  };
}

#endif // STEENX_VM_VM_H_
