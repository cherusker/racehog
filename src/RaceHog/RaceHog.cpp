#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>

#include "Displays/ConsoleDisplay/ConsoleDisplay.h"
#include "Displays/CountingDisplay/CountingDisplay.h"
#include "Filters/DuplicateFilter/DuplicateFilter.h"
#include "RaceHog/Module/DisplayModule.h"
#include "RaceHog/Module/FilterModule.h"
#include "RaceHog/Module/SearchSpaceBuilderModule.h"
#include "RaceHog/RaceHogCommandLine.h"
#include "SteenX/VM/SteenXBuilder.h"
#include "Support/Logging.h"
#include "Support/Stream/UserStream.h"

int
main(int          argc,
     const char** argv) {

  (void) racehog::initLogging(argv[0]);

  racehog::RaceHogCommandLine cmd(std::cerr);

  if (cmd.parse(argc, argv) == false)
    return EXIT_FAILURE;

  if (cmd.isHelp() == true) {
    (void) cmd.printUsage(std::cout);
    return EXIT_SUCCESS;
  }

  if (cmd.isVersion() == true) {
    (void) cmd.printVersion(std::cout);
    return EXIT_SUCCESS;
  }

  racehog::UserStream stream(std::cout);

  // Create modules:

  // TODO:  Find a uniersal way for setting up modules and configurations.

  racehog::SteenXBuilder steenXBuilder(
    stream, cmd.getFilename(), cmd.isInteractive());

  filters::DuplicateFilter duplicateFilter;
  displays::ConsoleDisplay consoleDisplay(stream);

  // Categorise + link modules:

  racehog::SearchSpaceBuilderModule*  builder = &steenXBuilder;
  std::vector<racehog::FilterModule*> filters({ &duplicateFilter });
  racehog::DisplayModule*             display = &consoleDisplay;

  std::vector<racehog::DataRaceQueue> queues(filters.size() + 1);
  (void) builder->setOutputDataRaceQueue(&queues.front());
  (void) display->setInputDataRaceQueue(&queues.back());
  for (int fid = 0, ff = filters.size(); fid < ff; ++fid) {
    (void) filters[fid]->setInputDataRaceQueue(&queues[fid]);
    (void) filters[fid]->setOutputDataRaceQueue(&queues[fid + 1]);
  }

  // Run modules:

#if 0

  (void) builder->run();
  for (auto f : filters)
    (void) f->run();
  (void) display->run();

#else

  std::vector<std::thread> threads;
  (void) threads.reserve(filters.size() + 2);
  (void) threads.emplace_back(&racehog::SearchSpaceBuilderModule::run, builder);
  (void) threads.emplace_back(&racehog::DisplayModule::run, display);
  for (auto f : filters)
    (void) threads.emplace_back(&racehog::FilterModule::run, f);
  for (auto& t : threads)
    (void) t.join();

#endif

  return EXIT_SUCCESS;
}
