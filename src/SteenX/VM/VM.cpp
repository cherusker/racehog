#include "SteenX/VM/VM.h"

#include "SteenX/VM/Execution.h"

bool
racehog::VM::
run(const Program& prog,
    DataRaceQueue& races) {
  if (checker.verify(prog) == false)
    return false;
  // TODO:  Start Execution in a worker thread.
  Execution exe(stream, races, prog);
  return exe.run();
};

bool
racehog::VM::
interactiveRun(const Program& prog,
               DataRaceQueue& races) {
  if (checker.verify(prog) == false)
    return false;
  Execution exe(stream, races, prog);
  return exe.interactiveRun();
};
