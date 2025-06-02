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
# Dependent Build Options
#--------------------------------------------------------------------
# ENABLE_BENCHMARK is only available if ENABLE_LOG is ON.
cmake_dependent_option(
    ENABLE_BENCHMARK    "Enable drawing performance benchmarking"  ON
    "ENABLE_LOG"        OFF
)

