#include "SteenX/Program/ProgramLinker.h"

void
racehog::ProgramLinker::
link(void) {
  // Link StringReference within MetadataSubpool:
  for (auto& meta : metapool) {
    (void) meta.setDirectory(strings[meta.getDirectory().getId()]);
    (void) meta.setFilename(strings[meta.getFilename().getId()]);
  }
  // Link StringReference within MetadataSubpool:
  for (auto& meta : namedmetapool) {
    (void) meta.setDirectory(strings[meta.getDirectory().getId()]);
    (void) meta.setFilename(strings[meta.getFilename().getId()]);
    (void) meta.setName(strings[meta.getName().getId()]);
  }
  // Link entry point:
  (void) prog.setEntryPoint(
    prog.getFunctionReference(prog.getEntryPoint().getId()));
  // Link functions:
  for (auto& func : prog.getFunctions()) {
    // Link MetadataReference within Function:
    const auto& funcmeta = func.getMetadata();
    if (funcmeta.isSet() == true)
      (void) func.setMetadata(namedmetapool[funcmeta.getId()]);
    for (auto& inst : func.getInstructions()) {
      const auto& instmeta = inst.getMetadata();
      if (instmeta.isSet() == true)
        (void) inst.setMetadata(metapool[instmeta.getId()]);
      for (auto& op : inst.getOperands())
        (void) dispatch(op);
    }
  }
}

void
racehog::ProgramLinker::
visitFunctionOperand(FunctionOperand& op) {
  (void) op.setFunction(prog.getFunctionReference(op.getFunction().getId()));
}
