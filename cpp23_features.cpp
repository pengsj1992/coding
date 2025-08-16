#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <ranges>
#include <algorithm>
#include <span>

// =============================================================================
// C++23 std::expected 模拟实现 (如果编译器还不支持)
// =============================================================================

// 简化版的expected实现，用于演示概念
template<typename T, typename E>
class expected {
private:
    bool has_val;
    union {
        T val;
        E err;
    };

public:
    expected(const T& value) : has_val(true), val(value) {}
    expected(const E& error) : has_val(false), err(error) {}
    
    ~expected() {
        if (has_val) {
            val.~T();
        } else {
            err.~E();
        }
    }
    
    bool has_value() const { return has_val; }
    
    const T& value() const { 
        if (!has_val) throw std::runtime_error("No value");
        return val; 
    }
    
    const E& error() const { 
        if (has_val) throw std::runtime_error("No error");
        return err; 
    }
    
    explicit operator bool() const { return has_val; }
};

// 使用expected的示例函数
expected<int, std::string> divide(int a, int b) {
    if (b == 0) {
        return expected<int, std::string>("Division by zero error");
    }
    return expected<int, std::string>(a / b);
}

void demonstrate_expected() {
    std::cout << "\n=== C++23 std::expected 演示 ===\n";
    
    auto result1 = divide(10, 2);
    if (result1) {
        std::cout << "10 / 2 = " << result1.value() << std::endl;
    } else {
        std::cout << "Error: " << result1.error() << std::endl;
    }
    
    auto result2 = divide(10, 0);
    if (result2) {
        std::cout << "10 / 0 = " << result2.value() << std::endl;
    } else {
        std::cout << "Error: " << result2.error() << std::endl;
    }
}

// =============================================================================
// C++23 std::mdspan 概念演示
// =============================================================================

// 简化版的mdspan概念演示
template<typename T>
class simple_mdspan_2d {
private:
    T* data_;
    size_t rows_, cols_;

public:
    simple_mdspan_2d(T* data, size_t rows, size_t cols) 
        : data_(data), rows_(rows), cols_(cols) {}
    
    T& operator()(size_t row, size_t col) {
        return data_[row * cols_ + col];
    }
    
    const T& operator()(size_t row, size_t col) const {
        return data_[row * cols_ + col];
    }
    
    size_t rows() const { return rows_; }
    size_t cols() const { return cols_; }
};

void demonstrate_mdspan() {
    std::cout << "\n=== C++23 std::mdspan 概念演示 ===\n";
    
    // 创建一个2D数组的数据
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    // 使用mdspan来访问2D数据
    simple_mdspan_2d<int> matrix(data.data(), 3, 3);
    
    std::cout << "3x3 矩阵:\n";
    for (size_t i = 0; i < matrix.rows(); ++i) {
        for (size_t j = 0; j < matrix.cols(); ++j) {
            std::cout << matrix(i, j) << " ";
        }
        std::cout << std::endl;
    }
    
    // 修改元素
    matrix(1, 1) = 99;
    std::cout << "修改后的矩阵[1][1] = " << matrix(1, 1) << std::endl;
}

// =============================================================================
// C++23 if consteval 演示
// =============================================================================

consteval int compile_time_factorial(int n) {
    if (n <= 1) return 1;
    return n * compile_time_factorial(n - 1);
}

constexpr int flexible_factorial(int n) {
    if consteval {
        // 编译时执行的代码
        return compile_time_factorial(n);
    } else {
        // 运行时执行的代码
        int result = 1;
        for (int i = 2; i <= n; ++i) {
            result *= i;
        }
        return result;
    }
}

void demonstrate_if_consteval() {
    std::cout << "\n=== C++23 if consteval 演示 ===\n";
    
    // 编译时计算
    constexpr int ct_result = flexible_factorial(5);
    std::cout << "编译时计算 5! = " << ct_result << std::endl;
    
    // 运行时计算
    int n = 6;
    int rt_result = flexible_factorial(n);
    std::cout << "运行时计算 6! = " << rt_result << std::endl;
}

// =============================================================================
// C++23 Ranges 改进演示
// =============================================================================

void demonstrate_ranges_improvements() {
    std::cout << "\n=== C++23 Ranges 改进演示 ===\n";
    
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // 使用ranges::to (C++23新增)
    // 注意：这可能需要更新的编译器支持
    std::cout << "原始数组: ";
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    // 链式操作
    auto processed = numbers 
        | std::views::filter([](int n) { return n % 2 == 1; })  // 奇数
        | std::views::transform([](int n) { return n * n; });   // 平方
    
    std::cout << "奇数的平方: ";
    for (int value : processed) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    
    // 使用std::ranges::fold (C++23概念)
    // 由于fold可能还不被广泛支持，我们使用传统方法演示概念
    int sum = 0;
    for (int value : processed) {
        sum += value;
    }
    std::cout << "奇数平方和: " << sum << std::endl;
}

// =============================================================================
// C++23 多维下标操作符演示
// =============================================================================

class Matrix {
private:
    std::vector<std::vector<int>> data;
    size_t rows_, cols_;

public:
    Matrix(size_t rows, size_t cols) : rows_(rows), cols_(cols) {
        data.resize(rows);
        for (auto& row : data) {
            row.resize(cols);
        }
    }
    
    // C++23 多维下标操作符
    int& operator[](size_t row, size_t col) {
        return data[row][col];
    }
    
    const int& operator[](size_t row, size_t col) const {
        return data[row][col];
    }
    
    // 传统的单维下标操作符（用于比较）
    std::vector<int>& operator[](size_t row) {
        return data[row];
    }
    
    size_t rows() const { return rows_; }
    size_t cols() const { return cols_; }
};

void demonstrate_multidimensional_subscript() {
    std::cout << "\n=== C++23 多维下标操作符演示 ===\n";
    
    Matrix matrix(3, 3);
    
    // 填充矩阵
    for (size_t i = 0; i < matrix.rows(); ++i) {
        for (size_t j = 0; j < matrix.cols(); ++j) {
            // 使用传统方法
            matrix[i][j] = static_cast<int>(i * 3 + j + 1);
        }
    }
    
    std::cout << "矩阵内容:\n";
    for (size_t i = 0; i < matrix.rows(); ++i) {
        for (size_t j = 0; j < matrix.cols(); ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    
    // C++23的多维下标操作符在GCC 13+中可能还不完全支持
    // 这里演示概念
    std::cout << "注意: C++23多维下标操作符 matrix[i,j] 语法可能需要更新的编译器支持\n";
}

// =============================================================================
// C++23 auto(x) 语法演示
// =============================================================================

void demonstrate_auto_syntax() {
    std::cout << "\n=== C++23 auto(x) 语法演示 ===\n";
    
    int x = 42;
    
    // C++23 允许使用 auto(x) 来创建副本
    // 等价于 auto temp = x;
    auto copy_x = x;  // 这是传统方法
    
    std::cout << "原值 x = " << x << std::endl;
    std::cout << "副本 copy_x = " << copy_x << std::endl;
    
    copy_x = 100;
    std::cout << "修改副本后: x = " << x << ", copy_x = " << copy_x << std::endl;
    
    // 在lambda中的应用示例
    auto lambda = [y = x](int z) {  // 等价于 [y = auto(x)]
        return y + z;
    };
    
    std::cout << "Lambda结果: " << lambda(10) << std::endl;
}

// =============================================================================
// 主函数
// =============================================================================

int main() {
    std::cout << "C++23 新特性演示程序\n";
    std::cout << "====================\n";
    
    demonstrate_expected();
    demonstrate_mdspan();
    demonstrate_if_consteval();
    demonstrate_ranges_improvements();
    demonstrate_multidimensional_subscript();
    demonstrate_auto_syntax();
    
    std::cout << "\n注意: 某些C++23特性可能需要最新的编译器版本才能完全支持。\n";
    std::cout << "建议使用 GCC 13+ 或 Clang 17+ 来编译这些示例。\n";
    
    return 0;
}