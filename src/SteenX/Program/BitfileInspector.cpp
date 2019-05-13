#include <cstdlib>
#include <iostream>

#include "SteenX/Program/Bitfile.h"
#include "SteenX/Program/BitfileInspectorCommandLine.h"
#include "SteenX/Program/Program.h"
#include "SteenX/Program/ProgramSyntaxChecker.h"
#include "Support/Logging.h"
#include "Support/Stream/UserStream.h"

int
main(int          argc,
     const char** argv) {
  
  racehog::initLogging(argv[0]);

  racehog::BitfileInspectorCommandLine cmd(std::cerr);
  if (cmd.parse(argc, argv) == false)
    return EXIT_FAILURE;

  racehog::UserStream stream(std::cout);
  racehog::Bitfile    file(cmd.getFilename());
  racehog::Program    prog;

  if (file.read(prog) == false) {
    stream.fatal() << "Unable to parse file '" << cmd.getFilename() << "'.\n";
    return EXIT_FAILURE;
  }

  racehog::ProgramSyntaxChecker checker(stream);
  (void) checker.verifyExhaustive(prog);

  (void) prog.print(stream);
  stream << "\n";

  return EXIT_SUCCESS;
}
