#--------------------------------------------------------------------
# Source and Header Files
#--------------------------------------------------------------------
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")

list(APPEND PUBLIC_HEADER_FILES
)

list(APPEND PRIVATE_HEADER_FILES
)

list(APPEND SOURCE_FILES
  # Modules
  ${SOURCE_DIR}/main.c
)

#--------------------------------------------------------------------
# Library Setup
#--------------------------------------------------------------------
add_executable(${PROJECT_NAME}
    # Public headers
    ${PUBLIC_HEADER_FILES}

    # Source files
    ${SOURCE_FILES}

    # Private headers
    ${PRIVATE_HEADER_FILES}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC ${LINK_DEPS})

# Set compile flags
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<OR:$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:AppleClang>>:-Weverything>
    $<$<C_COMPILER_ID:GNU>:-Wall -Wextra -fdiagnostics-color=always>
    $<$<C_COMPILER_ID:MSVC>:/W4 /WX>
)

#--------------------------------------------------------------------
# Include Directories
#--------------------------------------------------------------------
target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    "${INCLUDE_DEPS_DIR}"
)

#--------------------------------------------------------------------
# Target Properties
#--------------------------------------------------------------------
set_target_properties(${PROJECT_NAME}
PROPERTIES
        C_EXTENSIONS OFF
        C_STANDARD 99
        C_STANDARD_REQUIRED ON
)

#--------------------------------------------------------------------
# Compile definitions
#--------------------------------------------------------------------
target_compile_definitions(${PROJECT_NAME} PUBLIC
    # Log/Debug
    $<$<BOOL:${LOG_SUPPORT}>:LOG_SUPPORT>
)

#--------------------------------------------------------------------
# Source Groups
#--------------------------------------------------------------------
GroupSourcesByFolder(${PROJECT_NAME})

