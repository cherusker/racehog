#ifndef RACEHOG_SUPPORT_MACROS_H_
#define RACEHOG_SUPPORT_MACROS_H_

#include "Support/Logging.h"

#ifdef __has_attribute
#if __has_attribute(always_inline)
#define RH_ATTR_ALWAYS_INLINE __attribute__((always_inline))
#else // __has_attribute(always_inline)
#define RH_ATTR_ALWAYS_INLINE
#endif // __has_attribute(always_inline)
#else // __has_attribute
#define RH_ATTR_ALWAYS_INLINE
#endif // __has_attribute

#define RH_UNUSED(val)                          \
  do {                                          \
    (void) (val);                               \
  } while (0)

#define LOG_AND_RETURN_X_IF_X(FN_NAME, EXPR, BAD_EXPR, RETVAL)  \
  do {                                                          \
    if ((EXPR) == (BAD_EXPR)) {                                 \
      LOG(ERROR) << (FN_NAME) << " failed";                     \
      return (RETVAL);                                          \
    }                                                           \
  } while (0)

#define LOG_AND_RETURN_FALSE_IF_NULLPTR(FN_NAME, PTR)     \
  LOG_AND_RETURN_X_IF_X((FN_NAME), (PTR), nullptr, false)

#define LOG_AND_RETURN_FALSE_IF_FALSE(FN_NAME, EXPR)      \
  LOG_AND_RETURN_X_IF_X((FN_NAME), (EXPR), false, false)

#define LOG_AND_RETURN_FAILURE_IF_FALSE(FN_NAME, EXPR)          \
  LOG_AND_RETURN_X_IF_X((FN_NAME), (EXPR), false, EXIT_FAILURE)

#define LLVM_FATAL_EXIT(OBJ, MSG)                   \
  do {                                              \
    llvm::errs() << "Object: " << (OBJ) << "\n";    \
    (void) llvm::report_fatal_error((MSG), false);  \
  } while (0)

#define RH_FATAL_RETURN(LOGMSG, USERMSG)        \
  do {                                          \
    LOG(ERROR) << (LOGMSG);                     \
    stream.fatal() << (USERMSG) << "\n";        \
    return;                                     \
  } while (0)

#define RH_FATAL_RETURN_X(RETVAL, LOGMSG, USERMSG)  \
  do {                                              \
    LOG(ERROR) << (LOGMSG);                         \
    stream.fatal() << (USERMSG) << "\n";            \
    return (RETVAL);                                \
  } while (0)

/// Requires `stream`.

#define RH_FATAL_VOID_BROKEN_FILE(LOGMSG)                   \
  RH_FATAL_RETURN((LOGMSG), "Unable to read the input file.");

/// Requires `stream`.

#define RH_FATAL_VOID_BROKEN_PROGRAM(LOGMSG)                \
  RH_FATAL_RETURN((LOGMSG), "Unable to execute the program.");

/// Requires `stream`.

#define RH_FATAL_FALSE_BROKEN_PROGRAM(LOGMSG)                           \
  RH_FATAL_RETURN_X(false, (LOGMSG), "Unable to execute the program.");

// https://en.wikipedia.org/wiki/ANSI_escape_code

#define SGR_RESET             "\x1b[0m" // Reset EVERYthing.
#define SGR_BOLD              "\x1b[1m"
#define SGR_FAINT             "\x1b[2m" // Inverse of `SGR_BOLD`.
#define SGR_ITALIC            "\x1b[3m"
#define SGR_UNDERLINE         "\x1b[4m"
#define SGR_STRIKETHROUGH     "\x1b[9m"
#define SGR_UNDERLINE_OFF     "\x1b[24m"
#define SGR_ITALIC_OFF        "\x1b[27m"
#define SGR_STRIKETHROUGH_OFF "\x1b[29m"

#define SGR_FG_BLACK   "\x1b[30m"
#define SGR_FG_RED     "\x1b[31m"
#define SGR_FG_GREEN   "\x1b[32m"
#define SGR_FG_YELLOW  "\x1b[33m"
#define SGR_FG_BLUE    "\x1b[34m"
#define SGR_FG_MAGENTA "\x1b[35m"
#define SGR_FG_CYAN    "\x1b[36m"
#define SGR_FG_WHITE   "\x1b[37m"
#define SGR_FG_DEFAULT "\x1b[39m"

#define SGR_BG_BLACK   "\x1b[40m"
#define SGR_BG_RED     "\x1b[41m"
#define SGR_BG_GREEN   "\x1b[42m"
#define SGR_BG_YELLOW  "\x1b[43m"
#define SGR_BG_BLUE    "\x1b[44m"
#define SGR_BG_MAGENTA "\x1b[45m"
#define SGR_BG_CYAN    "\x1b[46m"
#define SGR_BG_WHITE   "\x1b[47m"
#define SGR_BG_DEFAULT "\x1b[49m"

#endif // RACEHOG_SUPPORT_MACROS_H_
