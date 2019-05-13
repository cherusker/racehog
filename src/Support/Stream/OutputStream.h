#ifndef RACEHOG_SUPPORT_STREAM_OUTPUTSTREAM_H_
#define RACEHOG_SUPPORT_STREAM_OUTPUTSTREAM_H_

#include <cstdint>
#include <ostream>

#include "Support/Logging.h"
#include "Support/Stream/Stream.h"

namespace racehog {
  
  /// Used for bit output operations such as writing Program into Bitfile.

  class OutputStream final
    : public Stream {

  public:
    
    OutputStream(std::ostream& stream)
      : stream(stream) {}

    /// Forwards `std::ostream::good`.

    bool
    good(void) const {
      return stream.good();
    }

    /// Write Stream::magic, Stream::major and Stream::minor.

    bool
    writeHeader(void) {
      (void) write<MagicNumber>(magic); // VLI8 would create 5 bytes here.
      (void) writeVLI8<VersionNumber>(major);
      (void) writeVLI8<VersionNumber>(minor);
      return good();
    }

    bool
    writeBool(bool val) {
      // VLI8 wouldn't help with `uint8_t`.
      return write<Bool>(val == true ? 1 : 0);
    }

    bool
    writeStringId(StringId id) {
      return writeVLI8<StringId>(id);
    }

    bool
    writeStringLength(StringLength len) {
      return writeVLI8<StringLength>(len);
    }

    /**
     * \param str Will be put to the underlaying `std::ostream` without the
     * trailing null terminator.
     */

    bool
    writeString(const std::string& str);

    bool
    writeFunctionId(FunctionId id) {
      return writeVLI8<FunctionId>(id);
    }

    bool
    writeFunctionArgumentId(FunctionArgumentId id) {
      return writeVLI8<FunctionArgumentId>(id);
    }

    bool
    writeInstructionId(InstructionId id) {
      return writeVLI8<InstructionId>(id);
    }

    bool
    writeInstructionType(InstructionType type) {
      // VLI8 wouldn't help with `uint8_t`.
      return write<InstructionType>(type);
    }

    bool
    writeOperandType(OperandType type) {
      // VLI8 wouldn't help with `uint8_t`.
      return write<OperandType>(type);
    }

    bool
    writeOperandId(OperandId id) {
      return writeVLI8<OperandId>(id);
    }

    bool
    writeMemoryAtomId(MemoryAtomId id) {
      return writeVLI8<MemoryAtomId>(id);
    }

    bool
    writeMemoryAtomPointerOffset(MemoryAtomPointerOffset offset) {
      return writeVLI8<MemoryAtomPointerOffset>(offset);
    }

    bool
    writeMetadataId(MetadataId id) {
      return writeVLI8<MetadataId>(id);
    }

    bool
    writeLine(Line line) {
      return writeVLI8<Line>(line);
    }

    bool
    writeColumn(Line col) {
      return writeVLI8<Column>(col);
    }

  private:

    std::ostream& stream;

    template<class T>
    bool
    write(T val) {
      if (stream.write(
            reinterpret_cast<const std::ostream::char_type*>(&val),
            sizeof(T)))
        return true;
      LOG(ERROR) << "std::ostream::write failed";
      return false;
    }

    /// Variable-length integer (chunks of 8 bits).
    /**
     * Idea:
     *
     *   - https://en.wikipedia.org/wiki/Variable-length_quantity
     *   - https://llvm.org/docs/BitCodeFormat.html#variable-width-value
     *
     * Bits:
     *
     *   - `7`: Technically the MSB (`0x80`); `1 ==` continuation, `0 ==` end
     *      here.
     *   - `6`: Actually used MSB for data (`0x7F`).
     *   - ...
     *   - `0`: Actually used LSB for data (`0x7F`).
     */

    template<class T>
    bool
    writeVLI8(T val) {
      // TODO: can we speed this up?
      VLI8Chunk chunk;
      do {
        chunk = val & 0x7F; // Extract the least significant 7 bits.
        val >>= 7;          // Drop the least significant 7 bits.
        if (val > 0)        // If more data left ...
          chunk |= 0x80;    // Set continuation bit.
        if (write<VLI8Chunk>(chunk) == false)
          return false;
      } while (val > 0);
      return true;
    }
  };
}

#endif // RACEHOG_SUPPORT_STREAM_OUTPUTSTREAM_H_
