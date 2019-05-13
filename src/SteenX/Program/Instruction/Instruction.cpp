#include "SteenX/Program/Instruction/Instruction.h"

#include "Support/Macros.h"

// TODO:  Use OperandVisitor and get rid of the switches?

#define RH_INSTRUCTION_READ_STREAM_CASE(TYPE)                       \
  case Operand::Type::TYPE:                                         \
  RH_ISTREAM_READ(                                                  \
    stream, TYPE ## Operand, ops.emplace_back<TYPE ## Operand>());  \
  break;

#define RH_INSTRUCTION_APPEND_OPERAND_CASE(TYPE)  \
  case Operand::Type::TYPE:                       \
  (void) ops.emplace_back<TYPE ## Operand>(       \
    *op.as<TYPE ## Operand>());                   \
  break;

bool
racehog::Instruction::
readFromStream(InputStream& stream) {
  RH_ISTREAM_READ(stream, MetadataReference, meta);
  auto num_ops = stream.readOperandId();
  while (num_ops-- > 0) {
    switch (static_cast<Operand::Type>(stream.readOperandType())) {
      RH_INSTRUCTION_READ_STREAM_CASE(Constant);
      RH_INSTRUCTION_READ_STREAM_CASE(Function);
      RH_INSTRUCTION_READ_STREAM_CASE(GlobalMemoryAtom);
      RH_INSTRUCTION_READ_STREAM_CASE(LocalMemoryAtom);
    default:
      LOG(ERROR) << "Unexpected operand type.";
      return false;
    }
  }
  return stream.allReadsGood();
}

bool
racehog::Instruction::
writeToStream(OutputStream& stream) const {
  RH_OSTREAM_WRITE(stream, MetadataReference, meta);
  (void) stream.writeOperandId(ops.size());
  for (const auto& op : ops) {
    (void) stream.writeOperandType(op.getType());
    RH_OSTREAM_WRITE(stream, Operand, op);
  }
  return stream.good();
}

void
racehog::Instruction::
append(const Operand& op) {
  switch (op.getType()) {
    RH_INSTRUCTION_APPEND_OPERAND_CASE(Constant);
    RH_INSTRUCTION_APPEND_OPERAND_CASE(Function);
    RH_INSTRUCTION_APPEND_OPERAND_CASE(GlobalMemoryAtom);
    RH_INSTRUCTION_APPEND_OPERAND_CASE(LocalMemoryAtom);
  default:
    LOG(ERROR) << "Unexpected operand type.";
  }
}
