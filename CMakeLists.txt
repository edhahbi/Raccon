cmake_minimum_required(VERSION 3.16)

project(MyProject LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Default to Debug if no build type is specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Collect source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*.c"
)

# Collect headers (for IDE visibility only)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/include/*.h"
)

# Test executable
add_executable(test
    test/test.c
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(test PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(test PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

# Debug flags
target_compile_options(test PRIVATE
    $<$<CONFIG:Debug>:-g3 -O0>
)

# Release flags
target_compile_options(test PRIVATE
    $<$<CONFIG:Release>:-O3>
)
