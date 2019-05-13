#ifndef STEENX_LLVM_INSTRUCTIONEXTRACTOR_H_
#define STEENX_LLVM_INSTRUCTIONEXTRACTOR_H_

#include <boost/core/noncopyable.hpp>

#include "llvm/IR/InstVisitor.h"

#include "SteenX/LLVM/AtomicLocksTable.h"
#include "SteenX/LLVM/CallHandler/CallHandlerRegistry.h"
#include "SteenX/LLVM/Memory/MemoryManager.h"
#include "SteenX/LLVM/MetadataExtractor.h"
#include "SteenX/LLVM/TypeSizesTable.h"
#include "SteenX/Program/Function.h"

namespace racehog {

  class InstructionExtractor final
    : public  llvm::InstVisitor<InstructionExtractor>,
      private boost::noncopyable {

  public:

    InstructionExtractor(Function&            func,
                         TypeSizesTable&      sizes,
                         AtomicLocksTable&    atomics,
                         OperandTranslator&   optrans,
                         MemoryManager&       locals,
                         CallHandlerRegistry& call_handlers,
                         MetadataExtractor&   metax)
      : func(func),
        sizes(sizes),
        atomics(atomics),
        optrans(optrans),
        locals(locals),
        call_handlers(call_handlers),
        metax(metax) {}

    /// Fallback;  will terminate with an error.

    void
    visitInstruction(llvm::Instruction& llvm_inst);

    void
    visitICmpInst(llvm::ICmpInst& llvm_inst) {
      (void) visitIgnoredInstImpl(llvm_inst);
    }
    
    void
    visitFCmpInst(llvm::FCmpInst& llvm_inst) {
      (void) visitIgnoredInstImpl(llvm_inst);
    }

    /// https://llvm.org/docs/LangRef.html#alloca-instruction

    void
    visitAllocaInst(llvm::AllocaInst& llvm_inst);

    void
    visitLoadInst(llvm::LoadInst& llvm_inst);

    void
    visitStoreInst(llvm::StoreInst& llvm_inst);

    void
    visitGetElementPtrInst(llvm::GetElementPtrInst& llvm_inst);

    /// https://llvm.org/docs/LangRef.html#cmpxchg-instruction

    void
    visitAtomicCmpXchgInst(llvm::AtomicCmpXchgInst& llvm_inst) {
      (void) visitAtomicInstImpl(llvm_inst, llvm_inst.getPointerOperand(), 2);
    }

    /// https://llvm.org/docs/LangRef.html#atomicrmw-instruction

    void
    visitAtomicRMWInst(llvm::AtomicRMWInst& llvm_inst) {
      (void) visitAtomicInstImpl(llvm_inst, llvm_inst.getPointerOperand(), 1);
    }

    void
    visitPHINode(llvm::PHINode& llvm_inst);

    void
    visitTruncInst(llvm::TruncInst& llvm_inst) {
      (void) visitSimpleCastInstImpl(llvm_inst);
    }

    void
    visitZExtInst(llvm::ZExtInst& llvm_inst) {
      (void) visitSimpleCastInstImpl(llvm_inst);
    }

    void
    visitSExtInst(llvm::SExtInst& llvm_inst) {
      (void) visitSimpleCastInstImpl(llvm_inst);
    }

    void
    visitFPTruncInst(llvm::FPTruncInst& llvm_inst) {
      (void) visitSimpleCastInstImpl(llvm_inst);
    }

    void
    visitFPExtInst(llvm::FPExtInst& llvm_inst) {
      (void) visitSimpleCastInstImpl(llvm_inst);
    }

    void
    visitFPToUIInst(llvm::FPToUIInst& llvm_inst) {
      (void) visitSimpleCastInstImpl(llvm_inst);
    }

    void
    visitFPToSIInst(llvm::FPToSIInst& llvm_inst) {
      (void) visitSimpleCastInstImpl(llvm_inst);
    }

    void
    visitUIToFPInst(llvm::UIToFPInst& llvm_inst) {
      (void) visitSimpleCastInstImpl(llvm_inst);
    }

    void
    visitSIToFPInst(llvm::SIToFPInst& llvm_inst) {
      (void) visitSimpleCastInstImpl(llvm_inst);
    }

    void
    visitBitCastInst(llvm::BitCastInst& llvm_inst) {
      (void) visitSimpleCastInstImpl(llvm_inst);
    }

    void
    visitCallInst(llvm::CallInst& llvm_inst);

    void
    visitReturnInst(llvm::ReturnInst& llvm_inst);

    void
    visitBranchInst(llvm::BranchInst& llvm_inst) {
      (void) visitIgnoredInstImpl(llvm_inst);
    }

    /// add, sub, mul, div, rem, shl, lshr, ashr, and, or, xor.

    void
    visitBinaryOperator(llvm::BinaryOperator& llvm_inst) {
      (void) visitIgnoredInstImpl(llvm_inst);
    }

    void
    visitExtractValueInst(llvm::ExtractValueInst& llvm_inst);

  private:
    
    Function&            func; ///< Actual Function to be built.
    TypeSizesTable&      sizes;
    AtomicLocksTable&    atomics;
    OperandTranslator&   optrans;
    MemoryManager&       locals;
    CallHandlerRegistry& call_handlers;
    MetadataExtractor&   metax;

    /// There are certain instructions that can be ignored:
    /**
     * Any instruction that operates on LLVM's "first class values" (with the
     * exception of pointers) and produces a _new_ and _independent_ "first
     * class value" only needs to register this new value (as it could be used
     * later on;  e.g. in `llvm::StoreInst`).
     *
     * Independency is the key factor here as e.g. cast instructions alias the
     * input and output value and, therefore, can _not_ be ignored.
     */

    void
    visitIgnoredInstImpl(llvm::Instruction& llvm_inst) {
      (void) locals.request(&llvm_inst);
    }

    /// Used for `llvm::AtomicCmpXchgInst` and `llvm::AtomicRMWInst` alike.
    /**
     * Value operands of those instruction can be ignored the same way as we
     * ignore the entirety of `llvm::BranchInst`.
     *
     * \param num_retvals Number of atoms required to store the return value;
     *                    for example, `llvm::AtomicCmpXchgInst` has two return
     *                    values.
     */

    // TODO:  Formally verify the claim about ignoring values.

    void visitAtomicInstImpl(llvm::Instruction&          llvm_inst,
                             llvm::Value*                llvm_ptr,
                             MemoryManager::MemoryAtomId num_retvals);

    /// Translation to EqualiseInstruction.
    /**
     * Note that even simple integer cast instructions like `llvm::ZExtInst`
     * have to be translated to EqualiseInstruction as they could contain
     * function pointer or thread pointers.
     */

    void
    visitSimpleCastInstImpl(llvm::CastInst& llvm_inst);
  };
}

#endif // STEENX_LLVM_INSTRUCTIONEXTRACTOR_H_
