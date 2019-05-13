#include "SteenX/VM/SteenXBuilder.h"

#include "SteenX/Program/Bitfile.h"
#include "SteenX/Program/Program.h"
#include "SteenX/VM/VM.h"

void
racehog::SteenXBuilder::
run(void) {

  auto&   out = getOutputDataRaceQueue();
  Bitfile file(filename);
  // Program prog; <-- reactive as soon as possible.

  if (file.read(prog) == false) {
    stream.fatal()
      << "Error parsing file: " << filename << "\n";
    (void) out.shutdown();
    return;
  }

  VM vm(stream);

  if (is_interactive == true)
    (void) vm.interactiveRun(prog, out);
  else
    (void) vm.run(prog, out);

  (void) out.shutdown();
}
