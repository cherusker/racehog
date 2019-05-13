#ifndef STEENX_LLVM_METADATAEXTRACTOR_H_
#define STEENX_LLVM_METADATAEXTRACTOR_H_

#include <boost/core/noncopyable.hpp>

#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"

#include "SteenX/LLVM/StringTable.h"
#include "SteenX/Program/Function.h"
#include "SteenX/Program/Instruction/Instruction.h"
#include "SteenX/Program/Metadata/Metadata.h"
#include "SteenX/Program/Metadata/MetadataPool.h"
#include "SteenX/Program/Metadata/MetadataSubpool.h"

namespace racehog {
  
  /// Extracts Metadata from various different LLVM objects.
  /**
   * Functions try to find Metadata.
   * If they succeed, they will copy relevant data into newly created Metadata
   * objects within their respective MetadataSubpool.
   * Functions return MetadataReference objects where MetadataReference::isSet
   * can be used to determine whether stuff was extracted or not.
   */

  class MetadataExtractor final
    : private boost::noncopyable {

  public:

    MetadataExtractor(StringTable&  strings,
                      MetadataPool& metapool)
      : strings(strings),
        plainpool(metapool.getMetadata()),
        namedpool(metapool.getNamedMetadata()) {}

    Function::MetadataReference
    extractFrom(const llvm::Function& func);

    Instruction::MetadataReference
    extractFrom(const llvm::Instruction& inst);

  private:
    
    StringTable&                    strings;
    MetadataSubpool<Metadata>&      plainpool;
    MetadataSubpool<NamedMetadata>& namedpool;
  };
}

#endif // STEENX_LLVM_METADATAEXTRACTOR_H_
