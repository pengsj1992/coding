cmake_minimum_required(VERSION 3.20)
project(CppNewFeaturesDemo VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 编译器特定设置
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # GCC 编译器设置
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "13.0")
        message(WARNING "GCC版本低于13.0，某些C++23特性可能不被支持")
    endif()
    
    # 启用必要的编译器标志
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    
    # 调试和发布模式设置
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
    
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Clang 编译器设置
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "17.0")
        message(WARNING "Clang版本低于17.0，某些C++23特性可能不被支持")
    endif()
    
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
    
elseif(MSVC)
    # MSVC 编译器设置
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19.30")
        message(WARNING "MSVC版本低于19.30，某些C++23特性可能不被支持")
    endif()
    
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    set(CMAKE_CXX_FLAGS_DEBUG "/Od /Zi")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")
endif()

# 检查编译器是否支持所需特性
include(CheckCXXSourceCompiles)

# 检查C++20 concepts支持
check_cxx_source_compiles("
#include <concepts>
template<typename T>
concept Integral = std::integral<T>;
int main() { return 0; }
" HAVE_CONCEPTS)

# 检查C++20 ranges支持
check_cxx_source_compiles("
#include <ranges>
#include <vector>
int main() {
    std::vector<int> v{1,2,3};
    auto r = v | std::views::filter([](int i){ return i > 1; });
    return 0;
}
" HAVE_RANGES)

# 检查C++20 coroutines支持
check_cxx_source_compiles("
#include <coroutine>
struct Task {
    struct promise_type {
        Task get_return_object() { return {}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };
};
Task func() { co_return; }
int main() { return 0; }
" HAVE_COROUTINES)

# 创建C++20特性演示可执行文件
add_executable(cpp20_demo cpp20_features.cpp)

# 设置目标属性
set_target_properties(cpp20_demo PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# 创建C++23特性演示可执行文件
add_executable(cpp23_demo cpp23_features.cpp)

# 设置目标属性
set_target_properties(cpp23_demo PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)

# 编译后的信息输出
if(HAVE_CONCEPTS)
    message(STATUS "C++20 Concepts: 支持")
else()
    message(WARNING "C++20 Concepts: 不支持")
endif()

if(HAVE_RANGES)
    message(STATUS "C++20 Ranges: 支持")
else()
    message(WARNING "C++20 Ranges: 不支持")
endif()

if(HAVE_COROUTINES)
    message(STATUS "C++20 Coroutines: 支持")
else()
    message(WARNING "C++20 Coroutines: 不支持")
endif()

# 安装规则（可选）
install(TARGETS cpp20_demo cpp23_demo
    RUNTIME DESTINATION bin
)

# 打印编译信息
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

# 为不同的构建类型添加自定义目标
add_custom_target(run_cpp20
    COMMAND cpp20_demo
    DEPENDS cpp20_demo
    COMMENT "运行C++20特性演示"
)

add_custom_target(run_cpp23
    COMMAND cpp23_demo
    DEPENDS cpp23_demo
    COMMENT "运行C++23特性演示"
)