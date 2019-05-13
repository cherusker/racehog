set("SOURCE_FILES"
  "${PROJECT_SOURCE_DIR}/Support/Logging.cpp"
  "${PROJECT_SOURCE_DIR}/Support/Stream/InputStream.cpp"
  "${PROJECT_SOURCE_DIR}/Support/Stream/OutputStream.cpp"
  "${PROJECT_SOURCE_DIR}/Support/Stream/UserStream.cpp")

add_library("racehog-support"     STATIC "${SOURCE_FILES}")
add_library("racehog-support-pic" STATIC "${SOURCE_FILES}")

set_property(TARGET "racehog-support-pic"
  PROPERTY "POSITION_INDEPENDENT_CODE" "ON")
