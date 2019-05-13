#ifndef RACEHOG_SUPPORT_USERSTREAM_H_
#define RACEHOG_SUPPORT_USERSTREAM_H_

#include <atomic>
#include <ostream>
#include <string>

#include <boost/core/noncopyable.hpp>

#include "Support/Macros.h"

namespace racehog {
  
  /// Information messages that are designed for users.
  /**
   * UserStream is designed to exist once within a program and to be shared
   * across different modules and different threads of a program.
   *
   * UserStream::hasFatalMessage can be used to detect and
   * synchronously/orderly abort the program in the presence of fatal issues
   * (this also works in multi-threaded environments).
   */

  class UserStream final
    : private boost::noncopyable {

  public:

    enum class Colour : char {
      Default = 0,
      Black   = 1,
      Red     = 2,
      Green   = 3,
      Yellow  = 4,
      Blue    = 5,
      Magenta = 6,
      Cyan    = 7,
      White   = 8
    };

    enum class Format : char {
      Default      = 0,
      Note         = 1,
      Warning      = 2,
      Error        = 3,
      Fatal        = 4,
      Keyword      = 5,
      Variable     = 6,
      MetaVariable = 7,
      String       = 8
    };

    /**
     * \param stream Will be used for printing.
     * \param no_formatting If `true`, UserStream::setFormat and
     *        UserStream::setForeground will not put any SGR strings on the
     *        stream.
     *        They will act like a `nop` (effectively returning "").
     */

    UserStream(std::ostream& stream,
               bool          no_formatting = false)
      : stream(stream),
        no_formatting(no_formatting),
        has_fatal_message(false) {}

    /// Forwards \p val to internal `std::ostream::operator<<`.

    template<class T>
    UserStream&
    operator<<(T val) {
      stream << val;
      return *this;
    }

    /// Thread-safe way to know if UserStream::fatal has been called yet.

    bool
    hasFatalMessage(void) const {
      return has_fatal_message.load(std::memory_order_relaxed);
    }

    /// `Note: ` will be put on the stream before returning it.

    UserStream&
    note(void) {
      return *this << this->setFormat(Format::Note)
                   << "Note"
                   << this->resetFormat()
                   << ": ";
    }

    /// `Warning: ` will be put on the stream before returning it.

    UserStream&
    warn(void) {
      return *this << this->setFormat(Format::Warning)
                   << "Warning"
                   << this->resetFormat()
                   << ": ";
    }

    /// `Error: ` will be put on the stream before returning it.

    UserStream&
    error(void) {
      return *this << this->setFormat(Format::Error)
                   << "Error"
                   << this->resetFormat()
                   << ": ";
    }

    /// Used for signalling fatal messages to potentially stop the program.
    /**
     * UserStream::hasFatalMessage will return `true` henceforth until the end
     * of time.
     */

    UserStream&
    fatal(void) {
      (void) has_fatal_message.store(true, std::memory_order_relaxed);
      return *this << this->setFormat(Format::Fatal)
                   << "Fatal"
                   << this->resetFormat()
                   << ": ";
    }

    /// Use SGR colours directly;  **use sparingly**!
    /**
     * Most of the time its better to use UserStream::setFormat as it will take
     * care of a unified UX.
     *
     * More useful hints at UserStream::setFormat.
     */

    const std::string&
    setForeground(Colour col) {
      return no_formatting == true ?
        noformat : foregrounds[static_cast<unsigned>(col)];
    }

    /// Shortcut for UserStream::setForeground with Colour::Default.

    const std::string&
    resetForeground(void) {
      return setForeground(Colour::Default);
    }

    /// Puts SGR codes on the stream if formatting is enabled for UserStream.
    /**
     * Make sure to unset foramts via UserStream::setFormat (using
     * Format::Default) and not, e.g. via UserStream::setForeground (using
     * Colour::Default) as the two "default" values are different.
     *
     * Never mix UserStream::setForeground and UserStream::setFormat;  always
     * properly reset one first and then use the other.
     */

    const std::string&
    setFormat(Format format) {
      return no_formatting == true ?
        noformat : formats[static_cast<unsigned>(format)];
    }

    /// Shortcut for UserStream::setFormat with Format::Default.

    const std::string&
    resetFormat(void) {
      return setFormat(Format::Default);
    }

  private:

    std::ostream&     stream;
    const bool        no_formatting;
    std::atomic<bool> has_fatal_message;

    static const std::string noformat; // Fallback.
    static const std::string foregrounds[];
    static const std::string formats[];
  };
}

#endif // RACEHOG_SUPPORT_USERSTREAM_H_
