cmake_minimum_required(VERSION 3.28)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#设置采用llvm编译工具链
set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

project(simx LANGUAGES CXX)

find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)

# 添加CLI11 - 推荐的参数解析库
include(FetchContent)
FetchContent_Declare(
    cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.4.1
)
FetchContent_MakeAvailable(cli11)

# 添加CLI11 - 推荐的参数解析库
include(FetchContent)
FetchContent_Declare(
    cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.4.1
)
FetchContent_MakeAvailable(cli11)

add_executable(${PROJECT_NAME} main.cpp init.cpp)



if (CMAKE_BUILD_TYPE STREQUAL Debug)
    add_definitions(-DDEBUG)
else()
    add_definitions(-DRELEASE)
endif()

#set_target_properties(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Debug)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog fmt::fmt CLI11::CLI11)
