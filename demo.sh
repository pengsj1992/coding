#!/bin/bash

# C++20/23 新特性演示运行脚本
# Demo script for C++20/23 features

echo "🚀 C++20 & C++23 新特性演示"
echo "============================="
echo ""

# 检查是否已构建
if [ ! -d "build" ] || [ ! -f "build/cpp20_demo" ] || [ ! -f "build/cpp23_demo" ]; then
    echo "📦 检测到需要构建项目..."
    if [ -f "./build.sh" ]; then
        echo "🔨 正在构建项目..."
        ./build.sh
        if [ $? -ne 0 ]; then
            echo "❌ 构建失败，退出演示"
            exit 1
        fi
    else
        echo "❌ 找不到构建脚本，请先运行构建"
        exit 1
    fi
fi

echo ""
echo "🎬 开始演示..."
echo ""

# 运行C++20演示
echo "📘 ===== C++20 新特性演示 ====="
echo ""
cd build
./cpp20_demo

echo ""
echo ""

# 运行C++23演示
echo "📗 ===== C++23 新特性演示 ====="
echo ""
./cpp23_demo

cd ..

echo ""
echo "🎉 演示完成！"
echo "=============="
echo ""
echo "📚 学习提示："
echo "   • 查看源代码文件来深入理解各个特性"
echo "   • 尝试修改代码参数来实验不同的行为"
echo "   • 参考 README.md 获取更多学习资源"
echo ""
echo "📁 文件结构："
echo "   cpp20_features.cpp  - C++20特性实现"
echo "   cpp23_features.cpp  - C++23特性实现"  
echo "   CMakeLists.txt      - 构建配置"
echo "   README.md           - 详细文档"
echo ""
echo "🔧 快速命令："
echo "   ./build.sh          - 重新构建项目"
echo "   cd build && make run_cpp20  - 运行C++20演示"
echo "   cd build && make run_cpp23  - 运行C++23演示"