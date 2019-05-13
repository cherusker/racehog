#ifndef STEENX_PROGRAM_PROGRAMLINKER_H_
#define STEENX_PROGRAM_PROGRAMLINKER_H_

#include <boost/core/noncopyable.hpp>

#include "SteenX/Program/Operand/OperandVisitor.h"
#include "SteenX/Program/Program.h"

namespace racehog {
  
  /// Wires Program after Program::readFromStream.
  /**
   * Links:
   *   - FunctionReference (Function from Program).
   *   - MetadataReference<Metadata> (Metadata from MetadataSubpool).
   *   - StringReference (`std::string` from StringPool).
   */

  class ProgramLinker final
    : public  OperandVisitor<ProgramLinker>,
      private boost::noncopyable {

  public:

    ProgramLinker(Program& prog)
      : prog(prog),
        strings(prog.getStringPool()),
        metapool(prog.getMetadataPool().getMetadata()),
        namedmetapool(prog.getMetadataPool().getNamedMetadata()) {}

    void
    link(void);

    void
    visitFunctionOperand(FunctionOperand& op);

  private:

    Program&    prog;
    StringPool& strings;

    MetadataSubpool<Metadata>&      metapool;
    MetadataSubpool<NamedMetadata>& namedmetapool;
  };
}

#endif // STEENX_PROGRAM_PROGRAMLINKER_H_
