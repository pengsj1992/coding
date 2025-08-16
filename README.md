# C++20 & C++23 新特性演示程序

🚀 这是一个全面演示C++20和C++23新特性的示例程序集合。通过实际的代码示例，帮助开发者理解和学习现代C++的最新功能。

## 📋 目录

- [特性概览](#特性概览)
- [环境要求](#环境要求)
- [快速开始](#快速开始)
- [C++20 新特性](#c20-新特性)
- [C++23 新特性](#c23-新特性)
- [编译说明](#编译说明)
- [常见问题](#常见问题)
- [学习资源](#学习资源)

## 🎯 特性概览

### C++20 主要特性
- ✅ **Concepts** - 模板约束和概念定义
- ✅ **Ranges** - 函数式编程风格的容器操作
- ✅ **Coroutines** - 协程支持，用于异步编程
- ✅ **Three-way Comparison** - 三路比较操作符 (`<=>`)
- ✅ **std::format** - 类型安全的字符串格式化
- ✅ **Modules** - 模块系统 (部分演示)

### C++23 主要特性
- ✅ **std::expected** - 错误处理的新方式
- ✅ **std::mdspan** - 多维数组视图
- ✅ **if consteval** - 编译时条件执行
- ✅ **Ranges改进** - 更多视图和算法
- ✅ **多维下标操作符** - `operator[](x, y)` 语法
- ✅ **auto(x)语法** - 显式类型推导

## 💻 环境要求

### 编译器支持

| 编译器 | 最低版本 | C++20支持 | C++23支持 | 推荐版本 |
|--------|----------|-----------|-----------|----------|
| GCC | 10.0 | 🟡 部分 | ❌ 无 | 13.0+ |
| GCC | 13.0+ | ✅ 完整 | ✅ 大部分 | **推荐** |
| Clang | 12.0 | 🟡 部分 | ❌ 无 | 17.0+ |
| Clang | 17.0+ | ✅ 完整 | ✅ 大部分 | **推荐** |
| MSVC | 19.30+ | 🟡 部分 | 🟡 部分 | 最新版 |

### 构建工具
- **CMake** 3.20+
- **Make** 或 **Ninja**

### 操作系统
- Linux (推荐)
- macOS
- Windows (需要适当的编译器)

## 🚀 快速开始

### 1. 克隆并进入目录
```bash
# 如果您已有代码文件，直接进入目录
cd cpp-modern-features-demo
```

### 2. 自动构建 (推荐)
```bash
# 使用构建脚本
./build.sh
```

### 3. 手动构建
```bash
# 创建构建目录
mkdir build && cd build

# 配置项目
cmake -DCMAKE_BUILD_TYPE=Release ..

# 编译
make -j$(nproc)
```

### 4. 运行演示
```bash
# 进入构建目录 (如果还没有)
cd build

# 运行C++20特性演示
./cpp20_demo

# 运行C++23特性演示
./cpp23_demo

# 运行完整演示
./combined_demo
```

## 📚 C++20 新特性

### 1. Concepts (概念)
```cpp
template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template<Numeric T>
T add(T a, T b) {
    return a + b;
}
```
**优势**: 更清晰的模板错误信息，更好的API文档化

### 2. Ranges (范围库)
```cpp
auto even_squared = numbers 
    | std::views::filter([](int n) { return n % 2 == 0; })
    | std::views::transform([](int n) { return n * n; });
```
**优势**: 函数式编程风格，更可读的代码，延迟计算

### 3. Coroutines (协程)
```cpp
Generator fibonacci_generator(int count) {
    int a = 0, b = 1;
    for (int i = 0; i < count; ++i) {
        co_yield a;
        int temp = a + b;
        a = b;
        b = temp;
    }
}
```
**优势**: 简化异步编程，生成器模式，状态机实现

### 4. Three-way Comparison (三路比较)
```cpp
struct Point {
    int x, y;
    auto operator<=>(const Point& other) const = default;
};
```
**优势**: 减少样板代码，自动生成所有比较操作符

## 🔮 C++23 新特性

### 1. std::expected (期望值)
```cpp
expected<int, std::string> divide(int a, int b) {
    if (b == 0) {
        return "Division by zero error";
    }
    return a / b;
}
```
**优势**: 更好的错误处理，避免异常开销，明确的错误类型

### 2. std::mdspan (多维跨度)
```cpp
simple_mdspan_2d<int> matrix(data.data(), 3, 3);
matrix(1, 2) = 42;  // 访问第1行第2列
```
**优势**: 标准化的多维数组访问，零开销抽象

### 3. if consteval (编译时条件)
```cpp
constexpr int factorial(int n) {
    if consteval {
        return compile_time_factorial(n);
    } else {
        return runtime_factorial(n);
    }
}
```
**优势**: 在编译时和运行时使用不同的算法，优化性能

## 🔧 编译说明

### CMake 目标

| 目标 | 描述 |
|------|------|
| `cpp20_demo` | 编译C++20特性演示 |
| `cpp23_demo` | 编译C++23特性演示 |
| `combined_demo` | 编译完整演示程序 |
| `run_cpp20` | 编译并运行C++20演示 |
| `run_cpp23` | 编译并运行C++23演示 |
| `run_all` | 编译并运行完整演示 |

### 编译选项
```bash
# 调试版本
cmake -DCMAKE_BUILD_TYPE=Debug ..

# 发布版本 (默认)
cmake -DCMAKE_BUILD_TYPE=Release ..

# 指定编译器
cmake -DCMAKE_CXX_COMPILER=g++-13 ..
cmake -DCMAKE_CXX_COMPILER=clang++-17 ..
```

## ❓ 常见问题

### Q: 编译时出现"concept not found"错误
**A**: 确保使用支持C++20的编译器版本 (GCC 10+ 或 Clang 12+)

### Q: 某些C++23特性无法编译
**A**: C++23特性需要最新的编译器。建议使用:
- GCC 13.0+
- Clang 17.0+

### Q: 如何在旧编译器上运行？
**A**: 程序包含了兼容性代码。某些特性会使用模拟实现。

### Q: Windows上如何编译？
**A**: 推荐使用:
- Visual Studio 2022 (最新版)
- MSYS2 + GCC 13+
- WSL2 + Ubuntu

## 📖 学习资源

### 官方文档
- [cppreference.com](https://cppreference.com) - C++标准库参考
- [isocpp.org](https://isocpp.org) - C++标准委员会
- [C++20 标准](https://www.iso.org/standard/79358.html)

### 在线工具
- [Compiler Explorer](https://compiler-explorer.com) - 在线编译器
- [C++ Insights](https://cppinsights.io) - C++代码解析工具
- [Quick Bench](https://quick-bench.com) - 性能基准测试

### 推荐书籍
- "C++20: The Complete Guide" by Nicolai M. Josuttis
- "Professional C++, 5th Edition" by Marc Gregoire
- "Effective Modern C++" by Scott Meyers

### 视频教程
- [CppCon](https://www.youtube.com/user/CppCon) - C++会议演讲
- [Jason Turner](https://www.youtube.com/user/lefticus1) - C++最佳实践
- [Back to Basics](https://www.youtube.com/playlist?list=PLHTh1InhhwT5o3GwbFYy3sR7HDNRA353e) - 基础概念

## 🤝 贡献

欢迎提交Issue和Pull Request！

### 贡献指南
1. Fork 项目
2. 创建特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 开启 Pull Request

## 📄 许可证

本项目采用 MIT 许可证 - 查看 [LICENSE](LICENSE) 文件了解详情。

## 🙏 致谢

感谢C++标准委员会和所有为现代C++发展做出贡献的开发者们！

---

**💡 提示**: 建议在学习时先运行演示程序，然后仔细阅读源代码，最后尝试修改参数来加深理解。

**🔔 注意**: 某些高级特性可能需要最新的编译器版本。如果遇到编译问题，请检查编译器版本并参考兼容性表格。