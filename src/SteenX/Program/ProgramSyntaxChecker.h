#ifndef STEENX_PROGRAM_PROGRAMANALYSIS_H_
#define STEENX_PROGRAM_PROGRAMANALYSIS_H_

#include <boost/core/noncopyable.hpp>

#include "SteenX/Program/Instruction/InstructionVisitor.h"
#include "SteenX/Program/Program.h"

namespace racehog {
  
  /// Program sntax checker.

  class ProgramSyntaxChecker final
    : public  InstructionVisitor<ProgramSyntaxChecker, bool>,
      private boost::noncopyable {

  public:

    ProgramSyntaxChecker(UserStream& stream)
      : stream(stream) {}

    /// Run a full syntax check on a Program.
    /**
     * This check includes:
     *
     *   - Make sure that all Operands are valid within an instruction (correct
     *     amount and types).
     *
     *   - Verify that all MemoryAtomOperand objects are within their
     *     respective (static) boundaries.
     *
     * If an issue is detected, ProgramSyntaxChecker::verify puts a useful
     * message to UserStream::fatal and stops right after the first reported
     * issue.
     */

    bool
    verify(const Program& prog) {
      return verifyImpl(prog);
    }

    /// Like ProgramSyntaxChecker::verify but ...
    /**
     * ... never stops and reports every issue it finds.
     */

    bool
    verifyExhaustive(const Program& prog) {
      return verifyImpl(prog, true);
    }

    bool
    visitInstruction(const Instruction& inst) {
      return report("Unexpected instruction", inst);
    }

    bool
    visitForkInstruction(const ForkInstruction& inst) {
      return verifyOperands(
        inst, {
          SyntaxOperandType::FunctionOrMemoryAtom,
          SyntaxOperandType::MemoryAtom
        }, true);
    }

    bool
    visitJoinInstruction(const JoinInstruction& inst) {
      return verifyOperands(
        inst, {
          SyntaxOperandType::MemoryAtom,
          SyntaxOperandType::MemoryAtom
        });
    }

    bool
    visitAcquireInstruction(const AcquireInstruction& inst) {
      return verifyOperands(inst, { SyntaxOperandType::MemoryAtomPointer });
    }

    bool
    visitReleaseInstruction(const ReleaseInstruction& inst) {
      return verifyOperands(inst, { SyntaxOperandType::MemoryAtomPointer });
    }

    bool
    visitCallInstruction(const CallInstruction& inst) {
      return verifyOperands(
        inst, {
          SyntaxOperandType::FunctionOrMemoryAtom,
          SyntaxOperandType::MemoryAtom
        }, true);
    }

    bool
    visitReturnInstruction(const ReturnInstruction& inst) {
      return verifyOperands(
        inst, { SyntaxOperandType::FunctionOrMemoryAtomOrMemoryAtomPointer });
    }

    bool
    visitReferenceInstruction(const ReferenceInstruction& inst) {
      return verifyOperands(
        inst, {
          SyntaxOperandType::MemoryAtom,
          SyntaxOperandType::MemoryAtomOrMemoryAtomPointer
        });
    }

    bool
    visitEqualiseInstruction(const EqualiseInstruction& inst) {
      return verifyOperands(
        inst, {
          SyntaxOperandType::FunctionOrMemoryAtom,
          SyntaxOperandType::MemoryAtomOrMemoryAtomPointer
        });
    }

    bool
    visitReadInstruction(const ReadInstruction& inst) {
      return verifyOperands(
        inst, { SyntaxOperandType::MemoryAtomOrMemoryAtomPointer });
    }

    bool
    visitWriteInstruction(const WriteInstruction& inst) {
      return verifyOperands(
        inst, { SyntaxOperandType::MemoryAtomOrMemoryAtomPointer });
    }

    bool
    visitAllocateInstruction(const AllocateInstruction& inst) {
      return verifyOperands(inst, { SyntaxOperandType::MemoryAtom });
    }

  private:

    enum class SyntaxOperandType : char {
      Anything                                = 0, ///< Anything.
      MemoryAtom                              = 1, ///< No pointer.
      MemoryAtomPointer                       = 2, ///< Only pointer.
      MemoryAtomOrMemoryAtomPointer           = 3, ///< Pointer or no pointer.
      FunctionOrMemoryAtom                    = 4, ///< No pointer.
      FunctionOrMemoryAtomOrMemoryAtomPointer = 5  ///< Pointer or no pointer.
    };

    typedef Stream::MemoryAtomId MemoryAtomId;
    typedef
      const std::initializer_list<SyntaxOperandType>
      SyntaxOperandTypes;

    UserStream& stream;

    MemoryAtomId num_global_atoms = 0;
    MemoryAtomId num_local_atoms  = 0;

    bool
    verifyImpl(const Program& prog,
               bool           is_exhaustive = false);

    /// Report an Instruction;  returns `false` for easy integration.

    bool
    report(const std::string& message,
           const Instruction& inst);

    /// Report an Operand;  returns `false` for easy integration.

    bool
    report(const std::string& message,
           const Operand&     op,
           const Instruction& inst);

    /**
     * \param has_opt_ops Expect (and ignore) additional, optional Operand
     *                    objects or report excessive Operand objects?
     */

    bool
    verifyOperands(const Instruction& inst,
                   SyntaxOperandTypes types,
                   bool               has_opt_ops = false);

    bool
    verifyOperand(const Instruction&      inst,
                  const Operand&          op,
                  const SyntaxOperandType type);

    bool
    verifyOperandBounds(const Instruction& inst,
                        const Operand&     op,
                        const MemoryAtomId num_atoms);
  };
}

#endif // STEENX_PROGRAM_PROGRAMANALYSIS_H_
