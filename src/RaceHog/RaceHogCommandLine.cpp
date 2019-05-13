#include "RaceHog/RaceHogCommandLine.h"

#include "Support/Logging.h"

racehog::RaceHogCommandLine::
RaceHogCommandLine(std::ostream& err)
  : err(err) {
  (void) basicOpts.add_options()
    ("help,h",    "Show usage message.")
    ("version,v", "Show version information.");
  (void) mainOpts.add_options()
    ("interactive,i", "Interactive mode.");
  (void) fileOpts.add_options()
    ("file,f",
     boost::program_options::value<std::string>()->required());
  (void) filePosOpts.add("file", 1);
}

bool
racehog::RaceHogCommandLine::
parse(int          argc,
      const char** argv) {
  boost::program_options::variables_map basicMap;
  (void) boost::program_options::store(
    boost::program_options::command_line_parser(argc, argv)
    .options(basicOpts)
    .allow_unregistered()
    .run(),
    basicMap);
  (void) boost::program_options::notify(basicMap);
  if (basicMap.count("help") == 1) {
    is_help = true;
    return true;
  }
  if (basicMap.count("version") == 1) {
    is_version = true;
    return true;
  }
  boost::program_options::variables_map mainMap;
  try {
    (void) boost::program_options::store(
      boost::program_options::command_line_parser(argc, argv)
      .options(mainOpts)
      .allow_unregistered()
      .run(),
      mainMap);
    (void) boost::program_options::store(
      boost::program_options::command_line_parser(argc, argv)
      .options(fileOpts)
      .positional(filePosOpts)
      .allow_unregistered()
      .run(),
      mainMap);
    (void) boost::program_options::notify(mainMap);
    if (mainMap.count("interactive") == 1)
      is_interactive = true;
    if (mainMap.count("file") != 1) {
      (void) printUsage(err);
      return false;
    }
    filename = mainMap["file"].as<std::string>();
  } catch (...) {
    // Boost spits exceptions left and right;  e.g. if arguments are malformed
    // or unknown.  We simply convert any kinds of issues into a usage message.
    (void) printUsage(err);
    return false;
  }
  return true;
}

void
racehog::RaceHogCommandLine::
printUsage(std::ostream& stream) const {
  stream << "\n"
         << "Usage:\n"
         << "  racehog [basic options]\n"
         << "  racehog [options] filename\n\n"
         << "Basic options:\n"
         << basicOpts << "\n"
         << "Options:\n"
         << mainOpts << "\n";
}

void
racehog::RaceHogCommandLine::
printVersion(std::ostream& stream) const {
  stream << "RaceHog 1.0.0\n";
}
