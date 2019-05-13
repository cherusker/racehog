#include "SteenX/Program/Program.h"

bool
racehog::Program::
readFromStream(InputStream& stream) {
  RH_ISTREAM_READ(stream, StringsPool,  strings);
  RH_ISTREAM_READ(stream, MetadataPool, metapool);
  num_global_atoms = stream.readMemoryAtomId();
  RH_ISTREAM_READ(stream, FunctionReference, entry);
  auto num_funcs = stream.readFunctionId();
  while (num_funcs-- > 0) {
    (void) funcs.emplace_back();
    RH_ISTREAM_READ(stream, Function, funcs.back());
  }
  return stream.allReadsGood();
}

bool
racehog::Program::
writeToStream(OutputStream& stream) const {
  RH_OSTREAM_WRITE(stream, StringsPool,  strings);
  RH_OSTREAM_WRITE(stream, MetadataPool, metapool);
  (void) stream.writeMemoryAtomId(num_global_atoms);
  RH_OSTREAM_WRITE(stream, FunctionReference, entry);
  (void) stream.writeFunctionId(funcs.size());
  for (const auto& func : funcs)
    RH_OSTREAM_WRITE(stream, Function, func);
  return stream.good();
}

void
racehog::Program::
print(UserStream&        stream,
      const std::string& indent) const {
  stream << indent << "Program:\n" << indent << "  Strings:";
  if (strings.empty() == true) {
    stream << " none";
  } else {
    stream << "\n";
    (void) strings.print(stream, indent + "    ");
  }
  stream << "\n";
  (void) metapool.print(stream, indent + "  ");
  stream << "\n"
         << indent << "  Global memory atoms: " << num_global_atoms << "\n"
         << indent << "  Entry point: "
         << stream.setFormat(UserStream::Format::Variable)
         << "F" << entry.getId()
         << stream.resetFormat()
         << "\n" << indent << "  Functions:";
  Function::Id func_id = 0;
  for (const auto& func : funcs) {
    stream << "\n" << indent << "    Function "
           << stream.setFormat(UserStream::Format::Variable)
           << "F" << func_id++
           << stream.resetFormat()
           << ":\n";
    (void) func.print(stream, indent + "      ");
  }
}
