#--------------------------------------------------------------------
# CMake Modules
#--------------------------------------------------------------------
include(CMakeDependentOption)

#--------------------------------------------------------------------
# Build Options
#--------------------------------------------------------------------
option(USE_CCACHE       "Enable compiler cache that can improve build times" ${IS_MAIN})
option(ENABLE_LOG       "Enable log support"                                 ON)

#--------------------------------------------------------------------
# Sanitize Options
#--------------------------------------------------------------------
# Configure sanitizer usage based on build type
#
# Supported sanitizer options:
#   - Address              : Detects memory corruption (buffer overflows, use-after-free)
#   - Undefined            : Detects undefined behavior (signed integer overflow, etc.)
#   - Thread               : Detects data races and thread safety issues
#   - Leak                 : Detects memory leaks
#   - 'Address;Undefined'  : Combines Address and Undefined sanitizers
#
# Usage: cmake -DUSE_SANITIZER=Address (or other options above)
#
# Build type behavior:
# - Debug builds: Defaults to 'Address;Undefined' (can be overridden)
# - Release/other builds: Automatically disabled (empty string) for performance
cmake_dependent_option(
    USE_SANITIZER                               # Option name
    "Enable sanitizers: Address | Undefined | Thread | Leak | 'Address;Undefined'"
    "'Address;Undefined'"                       # Default: Enable both sanitizers
    "CMAKE_BUILD_TYPE STREQUAL Debug"          # Condition: When Debug build
    ""                                         # Force: Disable in Release/other builds
)
