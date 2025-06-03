#--------------------------------------------------------------------
# CMake Modules
#--------------------------------------------------------------------
include(CMakeDependentOption)

#--------------------------------------------------------------------
# Build Options
#--------------------------------------------------------------------
option(USE_CCACHE       "Enable compiler cache that can improve build times" ${IS_MAIN})
option(ENABLE_LOG       "Enable log support"                                 ON)

