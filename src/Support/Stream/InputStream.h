#ifndef RACEHOG_SUPPORT_STREAM_INPUTSTREAM_H_
#define RACEHOG_SUPPORT_STREAM_INPUTSTREAM_H_

#include <istream>
#include <string>

#include "Support/Logging.h"
#include "Support/Stream/Stream.h"

namespace racehog {
  
  /// Input bit stream that, e.g., holds information to build a Program.

  class InputStream final
    : public Stream {

  public:

    InputStream(std::istream& stream)
      : stream(stream) {}

    ~InputStream(void) {
      delete[] strbuf;
    }

    /// Not equivalent to `istream::good`.
    /**
     * Returns `true` if all requested reads were successful and `false` if at
     * least one read ran into some kind of issue.
     *
     * Attention:  this behaviour is slightly different to `istream::good` as
     * `istream::good != istream::eof` holws always but
     * InputStream::allReadsGood can be equal to `istream::eof`.
     */

    bool
    allReadsGood(void) const {
      return all_reads_good;
    }

    /// Forwards `istream::eof`.

    bool
    eof(void) const {
      return stream.eof();
    }

    /// Read Stream::magic, Stream::major and Stream::minor.
    /**
     * Returns `true` if the stream is readable and all three elements
     * (Stream::magic, Stream::major and Stream::minor) match the expected
     * values.  Returns `false` otherwise.
     */

    bool
    readHeader(void) {
      return read<MagicNumber>(0)       == magic &&
             readVLI8<VersionNumber>(0) == major &&
             readVLI8<VersionNumber>(0) == minor &&
             allReadsGood()                  ? true : false;
    }

    /// Default (in case of an error):  `false`.

    bool
    readBool(void) {
      // VLI8 wouldn't help with `uint8_t`.
      return read<Bool>(0) == 1 ? true : false;
    }

    /// Default (in case of an error):  `0`.

    StringId
    readStringId(void) {
      return readVLI8<StringId>(0);
    }

    /// Default (in case of an error):  `0`.

    StringLength
    readStringLength(void) {
      return readVLI8<StringLength>(0);
    }

    /// Default (in case of an error):  empty string.

    std::string
    readString(StringLength len);

    /// Default (in case of an error):  `0`.

    FunctionId
    readFunctionId(void) {
      return readVLI8<FunctionId>(0);
    }

    /// Default (in case of an error):  `0`.

    FunctionArgumentId
    readFunctionArgumentId(void) {
      return readVLI8<FunctionArgumentId>(0);
    }

    /// Default (in case of an error):  `0`.

    InstructionId
    readInstructionId(void) {
      return readVLI8<InstructionId>(0);
    }

    /// Default (in case of an error):  InstructionType::Invalid.

    InstructionType
    readInstructionType(void) {
      // VLI8 wouldn't help with `uint8_t`.
      return read<InstructionType>(InstructionType::Invalid);
    }

    /// Default (in case of an error):  OperandType::Invalid.

    OperandType
    readOperandType(void) {
      // VLI8 wouldn't help with `uint8_t`.
      return read<OperandType>(OperandType::Invalid);
    }

    /// Default (in case of an error):  `0`.

    OperandId
    readOperandId(void) {
      return readVLI8<OperandId>(0);
    }

    /// Default (in case of an error):  `0`.

    MemoryAtomId
    readMemoryAtomId(void) {
      return readVLI8<MemoryAtomId>(0);
    }

    /// Default (in case of an error):  `0`.

    MemoryAtomPointerOffset
    readMemoryAtomPointerOffset(void) {
      return readVLI8<MemoryAtomPointerOffset>(0);
    }

    /// Default (in case of an error):  `0`.

    MetadataId
    readMetadataId(void) {
      return readVLI8<MetadataId>(0);
    }

    /// Default (in case of an error):  `0`.

    Line
    readLine(void) {
      return readVLI8<Line>(0);
    }

    /// Default (in case of an error):  `0`.

    Column
    readColumn(void) {
      return readVLI8<Column>(0);
    }

  private:

    std::istream& stream;

    bool all_reads_good = true;

    char*        strbuf     = nullptr; ///< Buffer for InputStream::readString.
    StringLength strbuf_len = 0;       ///< Length of InputStream::strbuf.

    template<class T>
    T
    read(T default_val) {
      T val;
      if (stream.read(
            reinterpret_cast<std::istream::char_type*>(&val),
            sizeof(T)))
        return val;
      LOG(ERROR) << "std::istream::read failed";
      all_reads_good = false;
      return default_val;
    }

    /// Variable-length integer (chunks of 8 bits).
    /**
     * See OutputStream::writeVLI8.
     */

    template<class T>
    T
    readVLI8(T default_val) {
      VLI8Chunk chunk;
      T         val    = 0; // Initialise the output with 0.
      T         temp;       // Temporary variable for shifting left.
      size_t    indent = 0; // Indent for shifting (increases with every chunk).
      do {                  //   Indent is `size_t` to match `sizeof`.
        // Note:  `sizeof` returns bytes but we discuss bits (8 bits = 1 byte).
        if (indent + 7 > sizeof(T) * 8) { // Out-of-bounds.
          all_reads_good = false;
          return default_val;
        }
        chunk = read<VLI8Chunk>(0);
        temp = chunk & 0x7F;            // Use the least significant 7 bits.
        temp <<= indent;                // Move the bits upwards.
        val |= temp;                    // Copy new bits to output.
        indent += 7;                    // Increasae the indent by 7 (bits).
      } while ((chunk & 0x80) == 0x80); // While continuation is set ...
      return val;
    }
  };
}

#endif // RACEHOG_SUPPORT_STREAM_INPUTSTREAM_H_
