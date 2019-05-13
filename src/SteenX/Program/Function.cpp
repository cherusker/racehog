#include "SteenX/Program/Function.h"

#include "SteenX/Program/Instruction/Instructions.h"
#include "Support/Macros.h"

#define RH_FUNCTION_READ_STREAM_CASE(TYPE)                        \
  case Instruction::Type::TYPE:                                   \
  RH_ISTREAM_READ(                                                \
    stream, TYPE ## Instruction, append<TYPE ## Instruction>());  \
  break;

bool
racehog::Function::
readFromStream(InputStream& stream) {
  RH_ISTREAM_READ(stream, MetadataReference, meta);
  num_local_atoms = stream.readMemoryAtomId();
  auto num_insts  = stream.readInstructionId();
  while (num_insts-- > 0) {
    switch (static_cast<Instruction::Type>(stream.readInstructionType())) {
      RH_FUNCTION_READ_STREAM_CASE(Fork);
      RH_FUNCTION_READ_STREAM_CASE(Join);
      RH_FUNCTION_READ_STREAM_CASE(Acquire);
      RH_FUNCTION_READ_STREAM_CASE(Release);
      RH_FUNCTION_READ_STREAM_CASE(Call);
      RH_FUNCTION_READ_STREAM_CASE(Return);
      RH_FUNCTION_READ_STREAM_CASE(Reference);
      RH_FUNCTION_READ_STREAM_CASE(Equalise);
      RH_FUNCTION_READ_STREAM_CASE(Read);
      RH_FUNCTION_READ_STREAM_CASE(Write);
      RH_FUNCTION_READ_STREAM_CASE(Allocate);
    default: {
      LOG(ERROR) << "Unexpected instruction type.";
      return false;
    }
    }
  }
  return stream.allReadsGood();
}

bool
racehog::Function::
writeToStream(OutputStream& stream) const {
  RH_OSTREAM_WRITE(stream, MetadataReference, meta);
  (void) stream.writeMemoryAtomId(num_local_atoms);
  (void) stream.writeInstructionId(insts.size());
  for (const auto& inst : insts) {
    (void) stream.writeInstructionType(inst.getType());
    RH_OSTREAM_WRITE(stream, Instruction, inst);
  }
  return stream.good();
}

void
racehog::Function::
print(UserStream&        stream,
      const std::string& indent) const {
  if (meta.isSet() == true) {
    stream << indent << "Name: "
           << stream.setFormat(UserStream::Format::String)
           << "\"" << *meta->getName() << "\" "
           << stream.resetFormat();
    (void) meta.print(stream); // Adds "!XX".
    stream << "\n";
  }
  stream << indent << "Memory atoms: " << num_local_atoms << "\n"
         << indent << "Instructions:";
  if (insts.empty() == true)
    stream << " none";
  for (const auto& inst : insts) {
    stream << "\n";
    (void) inst.print(stream, indent + "  ");
  }
}
