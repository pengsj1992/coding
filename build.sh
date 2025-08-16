#!/bin/bash

# C++20/23 新特性演示程序构建脚本
# Build script for C++20/23 features demonstration

echo "🚀 C++20/23 新特性演示程序构建脚本"
echo "======================================"

# 检查必要工具
check_tool() {
    if ! command -v "$1" &> /dev/null; then
        echo "❌ 错误: $1 未找到，请先安装"
        exit 1
    fi
}

echo "🔍 检查构建工具..."
check_tool cmake
check_tool make

# 检查编译器
if command -v g++ &> /dev/null; then
    GCC_VERSION=$(g++ --version | head -n1 | grep -o '[0-9]\+\.[0-9]\+' | head -1)
    echo "✅ 找到 GCC: $GCC_VERSION"
    
    # 修复版本比较逻辑
    MAJOR_VERSION=$(echo "$GCC_VERSION" | cut -d. -f1)
    if [ "$MAJOR_VERSION" -ge 13 ] 2>/dev/null; then
        echo "✅ GCC版本支持C++23特性"
        COMPILER="g++"
    elif [ "$MAJOR_VERSION" -ge 10 ] 2>/dev/null; then
        echo "⚠️  GCC版本支持C++20特性，部分C++23特性可能不支持"
        COMPILER="g++"
    else
        echo "❌ GCC版本过低，建议升级到13.0+"
        COMPILER="g++"  # 仍然尝试编译
    fi
elif command -v clang++ &> /dev/null; then
    CLANG_VERSION=$(clang++ --version | head -n1 | grep -o '[0-9]\+\.[0-9]\+' | head -1)
    echo "✅ 找到 Clang: $CLANG_VERSION"
    COMPILER="clang++"
else
    echo "❌ 未找到支持的C++编译器 (g++ 或 clang++)"
    exit 1
fi

# 设置构建目录
BUILD_DIR="build"
echo "📁 创建构建目录: $BUILD_DIR"

if [ -d "$BUILD_DIR" ]; then
    echo "🧹 清理旧的构建目录..."
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# 运行CMake配置
echo "⚙️  配置项目..."
if [ "$COMPILER" = "clang++" ]; then
    cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release ..
else
    cmake -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release ..
fi

if [ $? -ne 0 ]; then
    echo "❌ CMake配置失败"
    exit 1
fi

# 编译项目
echo "🔨 编译项目..."
make -j$(nproc)

if [ $? -eq 0 ]; then
    echo "✅ 编译成功!"
    echo ""
    echo "🎯 可执行文件已生成:"
    echo "   • cpp20_demo - C++20特性演示"
    echo "   • cpp23_demo - C++23特性演示"
    echo ""
    echo "▶️  运行示例:"
    echo "   cd build"
    echo "   ./cpp20_demo"
    echo "   ./cpp23_demo"
    echo ""
    echo "🔧 或使用make目标:"
    echo "   make run_cpp20"
    echo "   make run_cpp23"
else
    echo "❌ 编译失败"
    exit 1
fi

cd ..
echo "✨ 构建完成!"