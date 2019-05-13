cmake_minimum_required("VERSION" "3.0")

project("racehog")

set("CMAKE_ARCHIVE_OUTPUT_DIRECTORY" "${CMAKE_BINARY_DIR}/lib")
set("CMAKE_LIBRARY_OUTPUT_DIRECTORY" "${CMAKE_BINARY_DIR}/lib")
set("CMAKE_RUNTIME_OUTPUT_DIRECTORY" "${CMAKE_BINARY_DIR}/bin")

set("CMAKE_C_FLAGS"   "${CMAKE_C_FLAGS}   -Wall -pedantic -Wextra")
set("CMAKE_CXX_FLAGS" "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")

set("CMAKE_C_FLAGS_DEBUG"   "${CMAKE_C_FLAGS_DEBUG}   -O0")
set("CMAKE_CXX_FLAGS_DEBUG" "${CMAKE_CXX_FLAGS_DEBUG} -O0")

# LLVM prefers RTTI to be turned off.  To play things safe we should turn it off
#   for the whole project.  Unfortunately, argument parsing with boost has
#   issues with boost::any_cast<std::string> if RTTI is turned off (it _should_
#   not require RTTI but turns out 1.66 failes without RTTI).  Fortunately, LLVM
#   does not seem to care when confronted with RTTI which is why the following
#   lines are currently inactive.

# set("CMAKE_C_FLAGS"   "${CMAKE_C_FLAGS}   -fno-rtti")
# set("CMAKE_CXX_FLAGS" "${CMAKE_CXX_FLAGS} -fno-rtti")

include_directories("${PROJECT_SOURCE_DIR}")

enable_testing()

add_subdirectory("Displays")
add_subdirectory("Filters")
add_subdirectory("RaceHog")
add_subdirectory("SteenX")
add_subdirectory("Support")
