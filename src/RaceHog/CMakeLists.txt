set("SOURCE_DIR" "${PROJECT_SOURCE_DIR}/RaceHog")

add_library("racehog-utils" STATIC
  "${SOURCE_DIR}/DataRace/CallStackMetatrace.cpp"
  "${SOURCE_DIR}/DataRace/DataRace.cpp"
  "${SOURCE_DIR}/DataRace/DataRaceAccess.cpp"
  "${SOURCE_DIR}/DataRace/DataRaceQueue.cpp")

add_executable("racehog"
  "${SOURCE_DIR}/RaceHog.cpp"
  "${SOURCE_DIR}/RaceHogCommandLine.cpp")

target_link_libraries("racehog"
  "pthread"
  "glog"
  "consoledisplay"
  "countingdisplay"
  "duplicatefilter"
  "steenx-builder"  # link before `racehog-utils'
  "steenx-vm"
  "steenx-program"  # link before `racehog-support'
  "racehog-utils"   # link before `steenx-program'
  "racehog-support"
  "boost_program_options")
