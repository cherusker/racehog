#ifndef RACEHOG_SUPPORT_PRINTABLE_H_
#define RACEHOG_SUPPORT_PRINTABLE_H_

#include <string>

#include "Support/Stream/UserStream.h"

namespace racehog {
  
  /// Defines Printable::print for pretty-printing.

  class Printable {

  public:
    
    /**
     * The consumers of the stream of Printable::print are human users of the
     * program and not developers.
     * Use logging functions to talk to developers and make your best effort to
     * create useful and beautiful output for humans via Printable::print.
     *
     * No one expects Printable::print to be fast; rather make its code easy to
     * follow;  there's absolutely no point in racing it!
     *
     * Printable::print is only expected to put `\n` on the stream if its own
     * content spans across multiple lines.
     * A call to Printable::print should never end with `\n`.
     *
     * \param stream The stream to put output on.
     * \param indent String that has to be printed to the left on _every line_.
     *        This is important to preserve indents during multiline output and
     *        shall not be misued for cheap expected-same-line-prefixing that
     *        could break with a future update.
     */

    virtual void
    print(UserStream&        stream,
          const std::string& indent = "") const = 0;
  };
}

#endif // RACEHOG_SUPPORT_PRINTABLE_H_
