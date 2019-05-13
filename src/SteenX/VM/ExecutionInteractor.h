#ifndef STEENX_VM_EXECUTIONINTERACTOR_H_
#define STEENX_VM_EXECUTIONINTERACTOR_H_

#include <iostream>
#include <string>

#include <boost/core/noncopyable.hpp>

#include "SteenX/Program/Instruction/Instruction.h"
#include "SteenX/VM/Environment.h"

namespace racehog {
  
  /// Console utilities for Execution::interactiveRun.

  class ExecutionInteractor final
    : private boost::noncopyable {

  public:

    ExecutionInteractor(const Environment& env,
                        UserStream&        out,
                        std::istream&      in = std::cin)
      : env(env),
        out(out),
        in(in) {}

    /// Put a generic welcome message onto the output stream.

    void
    welcome(void);

    /// Put some text onto the output stream.
    /**
     * `\n` will be appended automatically.
     */

    void
    showText(const std::string& text);

    /// Put a description and an Instruction onto the output stream.
    /**
     * `\n` will be appended automatically.
     */

    void
    showInstruction(const std::string& desc,
                    const Instruction& inst);

    /// Interactive console that reads from the input stream.
    /**
     * \return `true` if the user wishes to execute the next instruction and
     *         `false` if the user wishes to stop the execution.
     */

    bool
    interact(void);

  private:

    const Environment& env;
    UserStream&        out;
    std::istream&      in;
    
    void
    usage(void) const;

    void
    printThreads(const Environment& env);

    void
    printMemoryStores(const Environment& env);
  };
}

#endif // STEENX_VM_EXECUTIONINTERACTOR_H_
