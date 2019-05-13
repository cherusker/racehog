#ifndef STEENX_PROGRAM_STRINGREFERENCE_H_
#define STEENX_PROGRAM_STRINGREFERENCE_H_

#include <boost/core/noncopyable.hpp>

#include "Support/Stream/Serialisable.h"
#include "Support/TrackingReference.h"

namespace racehog {

  /// TrackingReference, to connect `std::string` and StringPool.

  class StringReference final
    : public Serialisable,
      public TrackingReference<Stream::StringId, std::string> {

    typedef Stream::StringId IdT;
    typedef std::string      ReferenceT;

    typedef TrackingReference<IdT, ReferenceT> Super;

  public:

    StringReference(void) = default;

    StringReference(IdT         id,
                    ReferenceT* ref)
      : Super(id, ref) {}

    bool
    readFromStream(InputStream& stream) override {
      (void) setId(stream.readStringId());
      return stream.allReadsGood();
    }

    bool
    writeToStream(OutputStream& stream) const override {
      return stream.writeStringId(getId());
    }
  };
}

#endif // STEENX_PROGRAM_STRINGREFERENCE_H_
