#ifndef STEENX_PROGRAM_METADATA_METADATAREFERENCE_H_
#define STEENX_PROGRAM_METADATA_METADATAREFERENCE_H_

#include <boost/core/noncopyable.hpp>

#include "SteenX/Program/Metadata/Metadata.h"
#include "Support/OptionalTrackingReference.h"
#include "Support/Stream/Serialisable.h"

namespace racehog {

  /// OptionalTrackingReference, to connect Metadata and MetadataPool.

  template<class MetadataType>
  class MetadataReference final
    : public OptionalTrackingReference<Metadata::Id, MetadataType>,
      public Printable,
      public Serialisable {

  public:

    typedef Metadata::Id Id;

  private:

    typedef OptionalTrackingReference<Id, MetadataType> Super;
    typedef MetadataReference<MetadataType>             Self;

  public:

    MetadataReference(void) = default;

    MetadataReference(Id            id,
                      MetadataType* ref)
      : Super(id, ref) {}

    bool
    readFromStream(InputStream& stream) override {
      if (stream.readBool() == true)
        (void) Self::setId(stream.readMetadataId());
      return stream.allReadsGood();
    }

    bool
    writeToStream(OutputStream& stream) const override {
      (void) stream.writeBool(Self::isSet());
      if (Self::isSet())
        (void) stream.writeMetadataId(Self::getId());
      return stream.good();
    }

    /// Prints `!id` or nothing (not even \p indent), if not set.
    
    void
    print(UserStream&        stream,
          const std::string& indent = "") const override {
      if (Super::isSet() == true)
        stream << indent << "!"
               << stream.setFormat(UserStream::Format::MetaVariable)
               << Super::getId()
               << stream.resetFormat();
    }
  };
}

#endif // STEENX_PROGRAM_METADATA_METADATAREFERENCE_H_
