cmake_minimum_required(VERSION 3.26 FATAL_ERROR)
project(
  RayTracing
  VERSION 0.0.1
  DESCRIPTION "A simple raytracing"
  LANGUAGES C
  HOMEPAGE_URL "https://github.com/zschzen/RayTracing-One-Week"
)

# Determine if is the main project
set(IS_MAIN ${PROJECT_IS_TOP_LEVEL})

# --------------------------------------------------------------------
# Paths
# --------------------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# --------------------------------------------------------------------
# Options
# --------------------------------------------------------------------
include(CMakeOptions.cmake)

# --------------------------------------------------------------------
# Output Directories
# --------------------------------------------------------------------
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

# --------------------------------------------------------------------
# Dependencies
# --------------------------------------------------------------------
message(STATUS "========================================================================= ")
message(STATUS "> CPM.cmake")

include(cmake/CPM.cmake)
CPMUsePackageLock(package-lock.cmake)

include(cmake/tools.cmake)

CPMAddPackage("gh:TheLartians/GroupSourcesByFolder.cmake@1.0")

set(INCLUDE_DEPS_DIR "")
set(LINK_DEPS "")

list(APPEND INCLUDE_DEPS_DIR "${PROJECT_SOURCE_DIR}/external")

# --------------------------------------------------------------------
# Global Properties
# --------------------------------------------------------------------
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# --------------------------------------------------------------------
# Subdirectories
# --------------------------------------------------------------------
add_subdirectory(src raytracing)

message(STATUS "========================================================================= ")


