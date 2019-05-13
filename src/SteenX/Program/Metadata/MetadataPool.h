#ifndef STEENX_PROGRAM_METADATA_METADATAPOOL_H_
#define STEENX_PROGRAM_METADATA_METADATAPOOL_H_

#include <boost/core/noncopyable.hpp>

#include "SteenX/Program/Metadata/Metadata.h"
#include "SteenX/Program/Metadata/MetadataSubpool.h"
#include "SteenX/Program/Metadata/NamedMetadata.h"
#include "Support/Stream/Printable.h"
#include "Support/Stream/Serialisable.h"

namespace racehog {
  
  /// Collection of all kinds of MetadataSubpool.
  /**
   * There are a couple of reasons why we store objects of Metadata (far) away
   * from the objects that they actually describe.
   * The most important ones are:
   *
   *   - Data that is relevant to Execution (e.g. Function and Instruction)
   *     should be cached efficiently as it is accessed frequently.
   *     Metadata, on the other hand, is only used for debugging and reporting
   *     and most Metadata objects are never accessed.
   *
   *   - Not every object (Function, Instruction, ...) that could be described
   *     using Metadata is being described using Metadata.
   *     We want to avoid holding full Metadata objects that only say "I'm
   *     invalid".
   *     This means that described objects (Function, Instruction, ...) can only
   *     hold pointers to heap-allocated memory (MetadataReference).
   *     Since we also want to avoid to dynamically allocate memory explicitely,
   *     a memory pool feels like a legit solution.
   *
   *   - In addition to the point above, Metadata objects are also commonly
   *     shared between different Instruction objects (so it's really M:N).
   */

  class MetadataPool final
    : public  Printable,
      public  Serialisable,
      private boost::noncopyable {

  public:

    typedef MetadataSubpool<Metadata>      MetadataSub;
    typedef MetadataSubpool<NamedMetadata> NamedMetadataSub;

    bool
    readFromStream(InputStream& stream) override;

    bool
    writeToStream(OutputStream& stream) const override;

    MetadataSub&
    getMetadata(void) {
      return metadata;
    }

    NamedMetadataSub&
    getNamedMetadata(void) {
      return namedmetadata;
    }

    void
    print(UserStream&        stream,
          const std::string& indent = "") const override;

  private:

    MetadataSub      metadata;
    NamedMetadataSub namedmetadata;
  };
}

#endif // STEENX_PROGRAM_METADATA_METADATAPOOL_H_
