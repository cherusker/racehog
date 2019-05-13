#include "Support/Stream/UserStream.h"

const std::string
racehog::UserStream::
noformat = "";

const std::string
racehog::UserStream::
foregrounds[] = {
  SGR_FG_DEFAULT, // Default
  SGR_FG_BLACK,   // Black
  SGR_FG_RED,     // Red
  SGR_FG_GREEN,   // Green
  SGR_FG_YELLOW,  // Yellow
  SGR_FG_BLUE,    // Blue
  SGR_FG_MAGENTA, // Magenta
  SGR_FG_CYAN,    // Cyan
  SGR_FG_WHITE    // White
};

const std::string
racehog::UserStream::
formats[] = {
  SGR_RESET,               // Default
  SGR_BOLD SGR_FG_YELLOW,  // Note
  SGR_BOLD SGR_FG_YELLOW,  // Warning
  SGR_BOLD SGR_FG_RED,     // Error
  SGR_BOLD SGR_FG_RED,     // Fatal
  SGR_FG_GREEN,            // Keyword
  SGR_FG_RED,              // Variable
  SGR_FG_BLUE,             // MetaVariable
  SGR_FG_YELLOW            // String
};
