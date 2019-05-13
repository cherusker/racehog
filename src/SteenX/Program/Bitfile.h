#ifndef STEENX_PROGRAM_BITFILE_H_
#define STEENX_PROGRAM_BITFILE_H_

#include <string>

#include <boost/core/noncopyable.hpp>

#include "SteenX/Program/Program.h"

namespace racehog {
  
  /// Handles reading and writing files.

  class Bitfile final
    : private boost::noncopyable {

  public:

    Bitfile(const std::string& filename)
      : filename(filename) {}
    
    /// Read Program from a file.
    /**
     * Opens a file (specified by `filename` in Bitfile::Bitfile) and attempts
     * to read Program from it.
     *
     * \return `false` if that fails, `true` otherwise.
     */

    bool
    read(Program& program) const;

    /// Write Program into a file.
    /**
     * Opens a file (specified by `filename` in Bitfile::Bitfile) and attempts
     * to write Program into it.
     *
     * \return `false` if that fails, `true` otherwise.
     */

    bool
    write(const Program& program) const;

  private:

    const std::string& filename;
  };
}

#endif // STEENX_PROGRAM_BITFILE_H_
