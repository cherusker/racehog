#ifndef RACEHOG_SUPPORT_STREAM_SERIALISABLE_H_
#define RACEHOG_SUPPORT_STREAM_SERIALISABLE_H_

#include "Support/Stream/InputStream.h"
#include "Support/Stream/OutputStream.h"
#include "Support/Macros.h"

/// Bubble Serialisable::readFromStream up the inheritance ladder.

#define RH_ISTREAM_READUP(STREAM, CLASS)        \
  LOG_AND_RETURN_FALSE_IF_FALSE(                \
    #CLASS "::readFromStream",                  \
    CLASS::readFromStream(STREAM))              \

/// Bubble Serialisable::writeToStream up the inheritance ladder.

#define RH_OSTREAM_WRITEUP(STREAM, CLASS)       \
  LOG_AND_RETURN_FALSE_IF_FALSE(                \
    #CLASS "::writeToStream",                   \
    CLASS::writeToStream(STREAM))               \

/// Apply Serialisable::readFromStream with return-false-on-failure.

#define RH_ISTREAM_READ(STREAM, CLASS, VAR)     \
  LOG_AND_RETURN_FALSE_IF_FALSE(                \
    #CLASS "::readFromStream",                  \
    (VAR).readFromStream(STREAM));

/// Apply Serialisable::writeToStream with return-false-on-failure.

#define RH_OSTREAM_WRITE(STREAM, CLASS, VAR)    \
  LOG_AND_RETURN_FALSE_IF_FALSE(                \
    #CLASS "::writeToStream",                   \
    (VAR).writeToStream(STREAM));

namespace racehog {
  
  /// Interface for being able to handle InputStream and OutputStream.

  class Serialisable {

  public:
    
    /**
     * Callers can expect that InputStream is checked/validated before it is
     * used and, technically, _failed_ or _eof_ InputStream objects can be 
     * passed around without issues.
     *
     * `true` shall only be returned iff a callee was able to complete their
     * task on the given InputStream.
     * It is important to consider that `true` is not equivalent to a _good_
     * InputStream at any point in time but only marks a happy callee.
     *
     * Note:  even if an instance of InputStream was _good_ before calling 
     * Serialisable::readFromStream (which returned `true`), InputStream could
     * be _failed_ or _eof_ after returning to the caller.
     * Thus, always check InputStream before using it and don't make any
     * assumptions based on the return value of Serialisable::readFromStream.
     */

    virtual bool
    readFromStream(InputStream& stream) = 0;

    /// Same ideas as behind Serialisable::readFromStream.

    virtual bool
    writeToStream(OutputStream& stream) const = 0;
  };
}

#endif // RACEHOG_SUPPORT_STREAM_SERIALISABLE_H_
