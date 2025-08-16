cmake_minimum_required(VERSION 3.26)

project(starter LANGUAGES CXX VERSION 0.1.0)

# Generate compile_commands.json for clangd/clang-tidy tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build position independent (useful for shared libs, sanitizers, etc.)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Option to escalate warnings to errors during CI or local strict builds
option(ENABLE_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)

# Use ccache when available to speed up incremental builds
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif()

# Centralized warnings target to apply to all targets
add_library(project_warnings INTERFACE)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(project_warnings INTERFACE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wshadow
        -Wdouble-promotion
        -Woverloaded-virtual
        -Wnon-virtual-dtor
        -Wnull-dereference
        -Wimplicit-fallthrough
        -Wformat=2
        -fcolor-diagnostics
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(project_warnings INTERFACE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wshadow
        -Wdouble-promotion
        -Woverloaded-virtual
        -Wnon-virtual-dtor
        -Wnull-dereference
        -Wimplicit-fallthrough=3
        -Wformat=2
        -fdiagnostics-color=always
    )
elseif(MSVC)
    target_compile_options(project_warnings INTERFACE
        /W4
        /permissive-
        /Zc:preprocessor
        /EHsc
    )
endif()

if(ENABLE_WARNINGS_AS_ERRORS)
    if(MSVC)
        target_compile_options(project_warnings INTERFACE /WX)
    else()
        target_compile_options(project_warnings INTERFACE -Werror)
    endif()
endif()

# Example executable target
add_executable(starter_app src/main.cpp)

# Require C++23 without compiler-specific extensions
target_compile_features(starter_app PRIVATE cxx_std_23)
set_target_properties(starter_app PROPERTIES CXX_EXTENSIONS OFF)

# Apply warnings to the executable
target_link_libraries(starter_app PRIVATE project_warnings)

# Optionally enable clang-tidy if available (honors project/.clang-tidy)
find_program(CLANG_TIDY_EXE NAMES clang-tidy-19 clang-tidy)
if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXE})
endif()