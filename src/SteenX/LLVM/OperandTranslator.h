#ifndef STEENX_LLVM_OPERANDTRANSLATOR_H_
#define STEENX_LLVM_OPERANDTRANSLATOR_H_

#include <boost/core/noncopyable.hpp>

#include "llvm/IR/Constants.h"
#include "llvm/IR/Value.h"

#include "SteenX/LLVM/FunctionTable.h"
#include "SteenX/LLVM/GlobalsInitialiser.h"
#include "SteenX/LLVM/Memory/MemoryManager.h"
#include "SteenX/LLVM/TypeSizesTable.h"
#include "SteenX/Program/Function.h"
#include "SteenX/Program/Instruction/Instruction.h"

namespace racehog {

  /// Translates `llvm::Value` to Operand.
  /**
   * Note the factory design in its core:
   * use OperandTranslator::preload to receive a preloaded
   * OperandTranslator::Translation which can then be appended to an Instruction
   * (OperandTranslator::Translation::appendTo).
   *
   * Many factors lead to this design decision:
   *
   *   - OperandTranslator::Translation _has to_ be preloaded before being
   *     appended to an Instruction.
   *     Missing the preloading step should be made impossible.
   *
   *   - OperandTranslator::Translation requires a ton of different resources
   *     but only uses a few of those resources during each execution
   *     (varies a lot).
   *     Therefore, all those resources should be accessible through a
   *     middleman.
   *
   *   - Furthermore, requiring a huge amount of resources means big
   *     constructors which we do not want to write more often than necessary.
   *
   *   - In addition to requiring a lot of resources,
   *     OperandTranslator::Translation even requires some resources that not
   *     even InstructionExtractor (one of its main stakeholders) knows about.
   *     Those resources have to be set from somewhere "outside" and "smuggled"
   *     in through a blackbox (OperandTranslator).
   */

  class OperandTranslator final
    : private boost::noncopyable {

  public:

    class Translation final
      : private boost::noncopyable {

    public:

      Translation(OperandTranslator& optrans,
                  const llvm::Value* llvm_val)
        : optrans(optrans),
          llvm_val(llvm_val),
          is_const(llvm::isa<llvm::ConstantFP>(llvm_val)          == true ||
                   llvm::isa<llvm::ConstantInt>(llvm_val)         == true ||
                   llvm::isa<llvm::ConstantPointerNull>(llvm_val) == true ?
                   true : false) {}

      Translation(const Translation& other)
        : optrans(other.optrans),
          llvm_val(other.llvm_val),
          is_const(other.is_const) {
        LOG(INFO) << "Copying OperandTranslator::Translation.";
      }

      Translation(Translation&& other)
        : optrans(other.optrans),
          llvm_val(other.llvm_val),
          is_const(other.is_const) {
        other.llvm_val = nullptr; // Necessary?
        LOG(INFO) << "Moving OperandTranslator::Translation.";
      }

    public:

      /// There are a few special rules surrounding "primitive" constants.
      /**
       * Primitive constants are constants whose translation
       * (OperandTranslator::translate) results in ConstantOperand:
       * `llvm::Constant(FP | Int | PointerNull)`.
       * Knowing this upfront, can sometimes help with better IR emission
       * through skipping the creation of `nop` Instruction objects.
       */

      bool
      isConstantOperand(void) const {
        return is_const;
      }

      /// Preloads specific types of `llvm::Value` (and ignores the rest).
      /**
       * Translation::appendTo is used _after_ Instruction objects have been
       * appended to Function objects.
       * Some types of `llvm::Value` need special preparation steps before
       * usage, however, which is what Translator::preload provides:
       *
       *   - `llvm::ConstantExpr*`:
       *     Load `getelementptr` into local memory if the offset is `!= 0`.
       */

      bool
      preload(Function&                             func,
              const Instruction::MetadataReference& meta);

      /// Append `llvm::Value` as value Operand to an Instruction.

      bool
      appendAsValueTo(Instruction& inst) {
        return appendToImpl(inst);
      }

      /// Append `llvm::Value` as pointer MemoryOperand to an Instruction.
      /**
       * Returns `false` if appending would result in anything else but
       * MemoryOperand.
       */

      bool
      appendAsPointerTo(Instruction& inst) {
        return appendToImpl(inst, true);
      }

    private:

      OperandTranslator& optrans;
      const llvm::Value* llvm_val;
      bool               is_const;

      bool
      appendToImpl(Instruction& inst,
                   bool         is_ptr = false);
    };

    OperandTranslator(TypeSizesTable&     sizes,
                      FunctionTable&      funcs,
                      MemoryManager&      globals,
                      MemoryManager&      locals,
                      GlobalsInitialiser& ginit)
      : sizes(sizes),
        funcs(funcs),
        globals(globals),
        locals(locals),
        ginit(ginit) {}

    /// Most likely you are looking for OperandTranslator::preload.

    Translation
    create(const llvm::Value* llvm_val) {
      return Translation(*this, llvm_val);
    }

    /// Preload a `llvm::Value`.
    /**
     * Creates OperandTranslator::Translation and calls
     * OperandTranslator::Translation::preload on it before returning it.
     *
     * \param func See OperandTranslator::Translation::preload.
     * \param meta See OperandTranslator::Translation::preload.
     * \param llvm_val See OperandTranslator::Translation::Translation.
     */

    Translation
    preload(Function&                             func,
            const Instruction::MetadataReference& meta,
            const llvm::Value*                    llvm_val) {
      Translation t(*this, llvm_val);
      // TODO:  Propagate return value of Translation::preload to the caller.
      if (t.preload(func, meta) == false)
        LLVM_FATAL_EXIT(*llvm_val, "Translation::preload failed.");
      return t;
    }

  private:

    TypeSizesTable&     sizes;
    FunctionTable&      funcs;
    MemoryManager&      globals;
    MemoryManager&      locals;
    GlobalsInitialiser& ginit;
  };
}

#endif // STEENX_LLVM_OPERANDTRANSLATOR_H_
