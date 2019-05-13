#ifndef STEENX_PROGRAM_BITFILEINSPECTORCOMMANDLINE_H_
#define STEENX_PROGRAM_BITFILEINSPECTORCOMMANDLINE_H_

#include <ostream>
#include <string>

#include <boost/core/noncopyable.hpp>

namespace racehog {

  /// Handle command line arguments of BitfileInspector.

  class BitfileInspectorCommandLine final
    : private boost::noncopyable {

  public:
    
    /**
     * \param err The error channel to report usage to users.
     */

    BitfileInspectorCommandLine(std::ostream& err)
      : err(err) {}

    /// Parse command line arguments.
    /**
     * A usage message will be placed on the error stream if
     * BitfileInspectorCommandLine::parse returns `false`.
     */

    bool
    parse(int          argc,
          const char** argv);

    const std::string&
    getFilename(void) const {
      return filename;
    }

  private:

    std::ostream& err;
    std::string   filename;

    void
    printUsage(void);
  };
}

#endif // STEENX_PROGRAM_BITFILEINSPECTORCOMMANDLINE_H_
