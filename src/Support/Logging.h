#ifndef RACEHOG_SUPPORT_LOGGING_H_
#define RACEHOG_SUPPORT_LOGGING_H_

#define LOGGER_GLOG

// TODO:  Integrate `LOGGER_GLOG' into cmake ...

#ifdef LOGGER_GLOG

#include <glog/logging.h>

#else // LOGGER_GLOG

#include <iostream>

// Note:  The semicolon is important to preserve statements like:
// `if (foo) LOG(INFO) << "Hello world!";'

#define LOG(a)       ; if (0) std::cout
#define LOG_IF(a, b) ; if (0) std::cout

#endif // LOGGER_GLOG

namespace racehog {
  void
  initLogging(const char* exe_name);
}

#endif // RACEHOG_SUPPORT_LOGGING_H_
