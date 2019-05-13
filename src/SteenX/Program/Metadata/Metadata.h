#ifndef STEENX_PROGRAM_METADATA_METADATA_H_
#define STEENX_PROGRAM_METADATA_METADATA_H_

#include <boost/core/noncopyable.hpp>

#include "SteenX/Program/StringReference.h"
#include "Support/Stream/Printable.h"
#include "Support/Stream/Serialisable.h"

namespace racehog {
  
  /// A basic piece of metadata.

  class Metadata
    : public  Printable,
      public  Serialisable,
      private boost::noncopyable {

  public:

    typedef Stream::MetadataId Id;
    typedef Stream::Line       Line;
    typedef Stream::Column     Column;

    virtual
    ~Metadata(void) = default;

    bool
    readFromStream(InputStream& stream) override;

    bool
    writeToStream(OutputStream& stream) const override;

    /// Splitting paths into `filename` and `directory` keeps bitfiles smaller.

    const StringReference&
    getDirectory(void) const {
      return dir;
    }

    /// Splitting paths into `filename` and `directory` keeps bitfiles smaller.

    void
    setDirectory(const StringReference& dir) {
      this->dir = dir;
    }

    /// Splitting paths into `filename` and `directory` keeps bitfiles smaller.

    const StringReference&
    getFilename(void) const {
      return filename;
    }

    /// Splitting paths into `filename` and `directory` keeps bitfiles smaller.

    void
    setFilename(const StringReference& filename) {
      this->filename = filename;
    }

    Line
    getLine(void) const {
      return line;
    }

    void
    setLine(Line line) {
      this->line = line;
    }

    Column
    getColumn(void) const {
      return col;
    }

    void
    setColumn(Column col) {
      this->col = col;
    }

    void
    print(UserStream&        stream,
          const std::string& indent = "") const override;

  private:

    StringReference dir;
    StringReference filename;

    Line   line = 0;
    Column col  = 0;
  };
}

#endif // STEENX_PROGRAM_METADATA_METADATA_H_
