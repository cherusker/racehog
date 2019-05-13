#include "Support/Logging.h"
#include "Support/Macros.h"

void
racehog::
initLogging(const char* exe_name) {
#ifdef LOGGER_GLOG
  // 0 := INFO
  // 1 := WARNING
  // 2 := ERROR
  // 3 := FATAL
  ::FLAGS_minloglevel = 2;
  ::FLAGS_logtostderr = true;
  (void) google::InitGoogleLogging(exe_name);
  LOG(INFO) << "GLOG is online";
#else
  RACEHOG_UNDEFINED(exe_name);
#endif
}
