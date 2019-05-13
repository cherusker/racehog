#include "SteenX/VM/ExecutionInteractor.h"

void
racehog::ExecutionInteractor::
welcome(void) {
  out << "\n"
      << "-------------------------------------\n"
      << out.setForeground(UserStream::Colour::Yellow)
      << "  Interactive execution of RaceHog.\n"
      << out.resetForeground()
      << "-------------------------------------\n\n";
  (void) usage();
  out << "\n\n";
}

void
racehog::ExecutionInteractor::
showText(const std::string& text) {
  out << text << "\n";
}

void
racehog::ExecutionInteractor::
showInstruction(const std::string& desc,
                const Instruction& inst) {
  out << desc << ": ";
  (void) inst.print(out);
  out << "\n";
}

bool
racehog::ExecutionInteractor::
interact(void) {
  out << "\n";
  std::string cmd;
  for (;;) {
    out << "> ";
    std::getline(in, cmd);
    out << "\n";
    if (cmd.size() < 1)
      return true;
    if (cmd == "q")
      return false;
    if (cmd == "c")
      (void) env.getCallStackMetatrace().print(out);
    else if (cmd == "m")
      (void) printMemoryStores(env);
    else if (cmd == "t")
      (void) printThreads(env);
    else
      (void) usage();
    out << "\n\n";
  }
}

void
racehog::ExecutionInteractor::
usage(void) const {
  // TODO:  Replace "the next instruction" with the actual instruction or with
  //   "Give no command to exit RaceHog." if no instruction is left.
  out << "Possible commands:\n"
      << "  [t] Show threads\n"
      << "  [m] Show memory stores\n"
      << "  [c] Show call stack trace\n"
      << "  [q] Quit the execution\n\n"
      << "Give no command to execute the next instruction.";
}

void
racehog::ExecutionInteractor::
printThreads(const Environment& env) {
  const auto& tstack = env.getThreadStack();
  out << "Thread Stack:";
  if (tstack.size() == 0) {
    out << " empty";
  } else {
    // TODO:  Reverse the order?
    for (const auto t : tstack) {
      out << "\n";
      (void) t->print(out, "  ");
    }
  }
  const auto& threads = env.getThreads();
  out << "\n\nAll Threads:";
  if (threads.size() == 0) {
    out << " none";
  } else {
    for (const auto& t : threads) {
      out << "\n";
      (void) t.print(out, "  ");
    }
  }
}

void
racehog::ExecutionInteractor::
printMemoryStores(const Environment& env) {
  bool is_first_m = true;
  for (const auto& s : env.getMemoryStores()) {
    if (is_first_m == true)
      is_first_m = false;
    else
      out << "\n\n";
    out << "Memory Store " << s.getId() << ":";
    if (s.size() == 0) {
      out << " empty";
    } else {
      out << "\n";
      (void) s.print(out, "  ");
    }
  }
}
