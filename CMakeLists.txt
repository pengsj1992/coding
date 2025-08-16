cmake_minimum_required(VERSION 3.20)

project(cpp_new_features_demo LANGUAGES CXX)

# Prefer C++23 but allow fallback if compiler doesn't fully support it
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(cpp_new_features_demo
	src/main.cpp
)

# Threads for std::jthread
find_package(Threads REQUIRED)
target_link_libraries(cpp_new_features_demo PRIVATE Threads::Threads)

# Reasonable warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	target_compile_options(cpp_new_features_demo PRIVATE -Wall -Wextra -Wpedantic)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	target_compile_options(cpp_new_features_demo PRIVATE /W4)
endif()

# Some compilers need this to enable format/print in libstdc++ when available
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-fconstexpr-ops-limit=33554432" HAS_CONSTEXPR_LIMIT)
if (HAS_CONSTEXPR_LIMIT)
	target_compile_options(cpp_new_features_demo PRIVATE -fconstexpr-ops-limit=33554432)
endif()