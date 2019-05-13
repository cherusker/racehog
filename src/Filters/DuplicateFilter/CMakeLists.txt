set("SOURCE_DIR" "${PROJECT_SOURCE_DIR}/Filters/DuplicateFilter")

add_library("duplicatefilter" STATIC
  "${SOURCE_DIR}/DuplicateFilter.cpp")
