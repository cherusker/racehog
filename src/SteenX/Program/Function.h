#ifndef STEENX_PROGRAM_FUNCTION_H_
#define STEENX_PROGRAM_FUNCTION_H_

#include <boost/core/noncopyable.hpp>

#include "SteenX/Program/Instruction/Instructions.h"
#include "SteenX/Program/Metadata/MetadataReference.h"
#include "SteenX/Program/Metadata/NamedMetadata.h"
#include "Support/derivables.h"
#include "Support/range_iterator.h"
#include "Support/Stream/Printable.h"
#include "Support/Stream/Serialisable.h"

namespace racehog {
  
  class Function final
    : public  Printable,
      public  Serialisable,
      private boost::noncopyable {

    typedef derivables<Instruction,
                       ForkInstruction,
                       JoinInstruction,
                       CallInstruction,
                       ReturnInstruction,
                       ReferenceInstruction,
                       EqualiseInstruction,
                       ReadInstruction,
                       WriteInstruction,
                       AllocateInstruction> Instructions;

  public:

    typedef Stream::FunctionId               Id;
    typedef MetadataReference<NamedMetadata> MetadataReference;
    typedef Stream::MemoryAtomId             MemoryAtomId;

    typedef range_iterator<Instructions::iterator>       InstructionsRange;
    typedef range_iterator<Instructions::const_iterator> ConstInstructionsRange;

    bool
    readFromStream(InputStream& stream) override;

    bool
    writeToStream(OutputStream& stream) const override;

    const MetadataReference&
    getMetadata(void) const {
      return meta;
    }

    void
    setMetadata(const MetadataReference& meta) {
      this->meta = meta;
    }

    MemoryAtomId
    getNumLocalMemoryAtoms(void) const {
      return num_local_atoms;
    }

    void
    setNumLocalMemoryAtoms(MemoryAtomId num) {
      num_local_atoms = num;
    }

    Instruction::Id
    getNumInstructions(void) const {
      return insts.size();
    }

    /// No bounds check; know what you request!

    const Instruction&
    getInstruction(Instruction::Id id) const {
      return insts[id];
    }

    InstructionsRange
    getInstructions(void) {
      return InstructionsRange(insts);
    }

    ConstInstructionsRange
    getInstructions(void) const {
      return ConstInstructionsRange(insts);
    }

    /// Returns the appended Instruction.

    template<class    InstructionType,
             class... Args>
    InstructionType&
    append(Args&&... args) {
      return insts.emplace_back<InstructionType>(std::forward<Args>(args)...);
    }

    void
    print(UserStream&        stream,
          const std::string& indent = "") const override;

  private:

    MetadataReference meta;
    MemoryAtomId      num_local_atoms;
    Instructions      insts;
  };
}

#endif // STEENX_PROGRAM_FUNCTION_H_
