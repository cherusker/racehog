#ifndef RACEHOG_RACEHOGCOMMANDLINE_H_
#define RACEHOG_RACEHOGCOMMANDLINE_H_

#include <ostream>
#include <string>

#include <boost/core/noncopyable.hpp>
#include <boost/program_options.hpp>

namespace racehog {

  /// Handles command line arguments of RaceHog.

  class RaceHogCommandLine final
    : private boost::noncopyable {

  public:
    
    /**
     * \param err The error channel to report usage to users.
     */

    RaceHogCommandLine(std::ostream& err);

    /// Parsing can be expected successful IFF returns `true`.
    /**
     * RaceHogCommandLine talks to users directly (e.g. usage message or
     * version information) via `std::cerr`.
     * In such cases, RaceHogCommandLine::isComplete `== false`.
     */

    bool
    parse(int          argc,
          const char** argv);

    /// `-h [ --help ]`.

    bool
    isHelp(void) const {
      return is_help;
    }

    /// `-v [ --version ]`.

    bool
    isVersion(void) const {
      return is_version;
    }

    /// `-i [ --interactive ]`.

    bool
    isInteractive(void) const {
      return is_interactive;
    }

    const std::string&
    getFilename(void) const {
      return filename;
    }

    void
    printUsage(std::ostream& stream) const;

    void
    printVersion(std::ostream& stream) const;

  private:

    std::ostream& err;

    boost::program_options::options_description            basicOpts;
    boost::program_options::options_description            mainOpts;
    boost::program_options::options_description            fileOpts;
    boost::program_options::positional_options_description filePosOpts;

    bool        is_help        = false;
    bool        is_version     = false;
    bool        is_interactive = false;
    std::string filename;
  };
}

#endif // RACEHOG_RACEHOGCOMMANDLINE_H_
