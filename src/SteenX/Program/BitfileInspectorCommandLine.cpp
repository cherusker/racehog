#include "SteenX/Program/BitfileInspectorCommandLine.h"

bool
racehog::BitfileInspectorCommandLine::
parse(int          argc,
      const char** argv) {
  if (argc != 2) {
    (void) printUsage();
    return false;
  }
  (void) filename.append(argv[1]);
  return true;
}

void
racehog::BitfileInspectorCommandLine::
printUsage(void)  {
  err << "\n"
      << "Bitfile Inspector 1.0.0" << "\n\n"
      << "Usage: inspect filename" << "\n\n";
}
