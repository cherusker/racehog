#include "SteenX/Program/Bitfile.h"

#include <fstream>

#include "Support/Macros.h"
#include "Support/Stream/InputStream.h"
#include "Support/Stream/OutputStream.h"
#include "SteenX/Program/ProgramLinker.h"

#define CLOSE_FILE_AND_BAIL_OUT(FN_NAME, FILE, EXPR)  \
  do {                                                \
    if ((EXPR) == false) {                            \
      (void) (FILE).close();                          \
      LOG(ERROR) << (FN_NAME) << " failed";           \
      return false;                                   \
    }                                                 \
  } while (0)

bool
racehog::Bitfile::
read(Program& prog) const {
  std::ifstream file(filename, std::ios_base::binary | std::ios_base::in);
  LOG_AND_RETURN_FALSE_IF_FALSE("ifstream::is_open", file.is_open());
  CLOSE_FILE_AND_BAIL_OUT("ofstream::good", file, file.good());
  InputStream stream(file);
  CLOSE_FILE_AND_BAIL_OUT(
    "InputStream::readHeader", file, stream.readHeader());
  CLOSE_FILE_AND_BAIL_OUT(
    "Program::readFromStream", file, prog.readFromStream(stream));
  (void) file.close();
  ProgramLinker linker(prog);
  linker.link();
  return true;
}

bool
racehog::Bitfile::
write(const Program& prog) const {
  std::ofstream file(filename, std::ios_base::binary | std::ios_base::out);
  LOG_AND_RETURN_FALSE_IF_FALSE("ofstream::is_open", file.is_open());
  CLOSE_FILE_AND_BAIL_OUT("ofstream::good", file, file.good());
  OutputStream stream(file);
  CLOSE_FILE_AND_BAIL_OUT(
    "OutputStream::writeHeader", file, stream.writeHeader());
  CLOSE_FILE_AND_BAIL_OUT(
    "Program::writeToStream",    file, prog.writeToStream(stream));
  (void) file.close();
  return true;
}
