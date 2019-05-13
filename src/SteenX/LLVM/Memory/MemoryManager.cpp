#include "SteenX/LLVM/Memory/MemoryManager.h"

#include "SteenX/Program/Instruction/ReferenceInstruction.h"
#include "SteenX/Program/Operand/GlobalMemoryAtomOperand.h"
#include "SteenX/Program/Operand/LocalMemoryAtomOperand.h"

void
racehog::MemoryManager::
emplaceLocalReference(const Instruction::MetadataReference& meta,
                      MemoryAtomId                          ptr,
                      MemoryAtomId                          val) {
  auto& ref = func->append<ReferenceInstruction>(meta);
  (void) ref.append<LocalMemoryAtomOperand>(ptr);
  (void) ref.append<LocalMemoryAtomOperand>(val);
}

void
racehog::MemoryManager::
emplaceGlobalReference(MemoryAtomId ptr,
                       MemoryAtomId val) {
  auto& ref = func->append<ReferenceInstruction>();
  (void) ref.append<GlobalMemoryAtomOperand>(ptr);
  (void) ref.append<GlobalMemoryAtomOperand>(val);
}
