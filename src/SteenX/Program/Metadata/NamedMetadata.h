#ifndef STEENX_PROGRAM_METADATA_NAMEDMETADATA_H_
#define STEENX_PROGRAM_METADATA_NAMEDMETADATA_H_

#include "SteenX/Program/Metadata/Metadata.h"

namespace racehog {
  
  /// A piece of Metadata, with a name.

  class NamedMetadata final
    : public Metadata {

  public:

    bool
    readFromStream(InputStream& stream) override;

    bool
    writeToStream(OutputStream& stream) const override;

    const StringReference&
    getName(void) const {
      return name;
    }

    /// See Metadata::setDirectoryId.

    void
    setName(const StringReference& name) {
      this->name = name;
    }

    void
    print(UserStream&        stream,
          const std::string& indent = "") const override;

  private:

    StringReference name;
  };
}

#endif // STEENX_PROGRAM_METADATA_NAMEDMETADATA_H_
