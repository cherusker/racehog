#ifndef RACEHOG_SUPPORT_STREAM_STREAM_H_
#define RACEHOG_SUPPORT_STREAM_STREAM_H_

#include <cstdint>

#include <boost/core/noncopyable.hpp>

namespace racehog {
  
  /// Collection of many important typedefs.
  /**
   * Collecting the typedefs in Stream has the benefit of dealing with
   * `<cstdint>` only once.
   * In addition, all changes to bitstream type sizes can be dealt with from
   * one-and-only-one place (Stream).
   *
   * Also, (almost) all affected classes implement the Streamable interface
   * which means they include the Stream header implicitly.
   * These classes are further welcome to retype those types if convenient (see
   *  e.g. Function::Id or Operand::Type).
   *
   * All that being said, cleverer + better ideas are always welcome :)
   */

  class Stream
    : private boost::noncopyable {

  public:

    /// C++'s `bool` values are handled as `uint8_t` in streams.
    /**
     * Stream::Bool value of `1` equals C++'s `true`, any other value equals
     * `false`.
     */

    // TODO:  Convert booleans to single bit in streams (requires enhanced
    //   stream logic as we always read 8 bits at a time)?

    typedef uint8_t Bool; 

    typedef uint32_t StringId;
    typedef uint16_t StringLength;

    typedef uint16_t FunctionId;
    typedef uint16_t FunctionArgumentId;

    typedef uint16_t InstructionId;

    enum class InstructionType : uint8_t {
      Invalid   =  0, ///< Indicates errors / uninitialised values.
      Fork      =  1, ///< ForkInstruction.
      Join      =  2, ///< JoinInstruction.
      Acquire   =  3, ///< AcquireInstruction.
      Release   =  4, ///< ReleaseInstruction.
      Call      =  5, ///< CallInstruction.
      Return    =  6, ///< ReturnInstruction.
      Equalise  =  7, ///< EqualiseInstruction.
      Read      =  8, ///< ReadInstruction.
      Write     =  9, ///< WriteInstruction.
      Reference = 10, ///< ReferenceInstruction.
      Allocate  = 11  ///< AllocateInstruction.
    };

    typedef uint32_t MemoryAtomId;
    typedef int64_t  MemoryAtomPointerOffset; ///< Can be negative.

    enum class OperandType : uint8_t {
      Invalid          = 0, ///< Indicates errors / uninitialised values.
      Constant         = 1, ///< ConstantOperand.
      Function         = 2, ///< FunctionOperand.
      MemoryAtom       = 3, ///< MemoryAtomOperand.
      GlobalMemoryAtom = 4, ///< GlobalMemoryAtomOperand.
      LocalMemoryAtom  = 5  ///< LocalMemoryAtomOperand.
    };

    typedef uint16_t OperandId;

    typedef uint32_t MetadataId;
    typedef uint32_t Line;
    typedef uint16_t Column;

    typedef uint16_t ThreadId;

    enum class AccessMode : uint8_t {
      Invalid = 0, ///< Indicates errors / uninitialised values.
      Read    = 1, ///< Read access.
      Write   = 2  ///< Write access.
    };

  protected:

    typedef uint32_t MagicNumber;
    typedef uint16_t VersionNumber;

    typedef uint8_t VLI8Chunk; ///< VLI8 base type.

    const MagicNumber   magic = 0x52414345; ///< "RACE" ;)
    const VersionNumber major = 0;
    const VersionNumber minor = 1;

    /// Only used as base class and statically for its typedefs.

    Stream(void) {}
  };
}

#endif // RACEHOG_SUPPORT_STREAM_STREAM_H_
