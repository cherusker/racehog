#include "SteenX/LLVM/MetadataExtractor.h"

#include "llvm/IR/DebugInfoMetadata.h"

// TODO:  Avoid (some of the) duplicate code in here?

racehog::Function::MetadataReference
racehog::MetadataExtractor::
extractFrom(const llvm::Function& func) {
  const auto sub = func.getSubprogram();
  if (sub == nullptr)
    return Function::MetadataReference();
  auto meta = namedpool.create();
  (void) meta->setDirectory(strings.insert(sub->getDirectory()));
  (void) meta->setFilename(strings.insert(sub->getFilename()));
  (void) meta->setLine(sub->getLine());
  (void) meta->setColumn(0); // We don't extract this information (yet).
  (void) meta->setName(strings.insert(sub->getName()));
  return meta;
}

racehog::Instruction::MetadataReference
racehog::MetadataExtractor::
extractFrom(const llvm::Instruction& inst) {
  const auto loc = inst.getDebugLoc().get();
  if (loc == nullptr)
    return Instruction::MetadataReference(); // No metadata available.
  const auto scope = loc->getScope();
  if (scope == nullptr)
    return Instruction::MetadataReference(); // No scope, no data.
  auto meta = plainpool.create();
  (void) meta->setDirectory(strings.insert(scope->getDirectory()));
  (void) meta->setFilename(strings.insert(scope->getFilename()));
  (void) meta->setLine(loc->getLine());
  (void) meta->setColumn(loc->getColumn());
  return meta;
}
