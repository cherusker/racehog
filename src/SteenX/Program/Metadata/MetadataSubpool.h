#ifndef STEENX_PROGRAM_METADATA_METADATASUBPOOL_H_
#define STEENX_PROGRAM_METADATA_METADATASUBPOOL_H_

#include <deque>

#include <boost/core/noncopyable.hpp>

#include "SteenX/Program/Metadata/MetadataReference.h"
#include "Support/Stream/Printable.h"

namespace racehog {
  
  /// Holds a certain kind of Metadata objects.

  template<class MetadataT>
  class MetadataSubpool final
    : public  Printable,
      public  Serialisable,
      private boost::noncopyable {

    /// We require a datastructure that does not move placed objects ever.

    typedef std::deque<MetadataT> DataContainer;

  public:

    typedef MetadataReference<MetadataT>     MetadataReference;
    typedef typename DataContainer::iterator iterator;

    /// No bounds check;  know what you request!

    MetadataReference
    operator[](Metadata::Id id) {
      return MetadataReference(id, &data[id]);
    }

    bool
    readFromStream(InputStream& stream) override {
      auto num = stream.readMetadataId();
      while (num-- > 0) {
        (void) data.emplace_back();
        RH_ISTREAM_READ(stream, Metadata, data.back());
      }
      return stream.allReadsGood();
    }

    bool
    writeToStream(OutputStream& stream) const override {
      (void) stream.writeMetadataId(data.size());
      for (const auto& meta : data)
        RH_OSTREAM_WRITE(stream, Metadata, meta);
      return stream.good();
    }

    /// Returns a new Metadata object that is managed by MetadataSubpool.

    MetadataReference
    create(void) {
      // We expect no fails whatsoever ...
      (void) data.emplace_back();
      return MetadataReference(data.size() - 1, &data.back());
    }

    iterator
    begin(void) {
      return data.begin();
    }

    iterator
    end(void) {
      return data.end();
    }

    bool
    empty(void) const {
      return data.empty();
    }

    Metadata::Id
    size(void) const {
      return data.size();
    }

    void
    print(UserStream&        stream,
          const std::string& indent = "") const override {
      bool is_first = true;
      Metadata::Id id = 0;
      for (const auto& meta : data) {
        if (is_first == true)
          is_first = false;
        else
          stream << "\n";
        stream << indent << "  "
               << stream.setFormat(UserStream::Format::MetaVariable)
               << id++
               << stream.resetFormat()
               << ": ";
        (void) meta.print(stream);
      }
    }

  private:

    DataContainer data;
  };
}

#endif // STEENX_PROGRAM_METADATA_METADATASUBPOOL_H_
