#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>
#include <concepts>
#include <coroutine>
#include <string>
#include <compare>
#include <format>

// =============================================================================
// C++20 Concepts 演示
// =============================================================================

// 定义一个concept来约束数字类型
template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

// 使用concept的函数模板
template<Numeric T>
T add(T a, T b) {
    return a + b;
}

// 更复杂的concept
template<typename T>
concept Printable = requires(T t) {
    std::cout << t;
};

template<Printable T>
void print_value(const T& value) {
    std::cout << "Value: " << value << std::endl;
}

// =============================================================================
// C++20 Ranges 演示
// =============================================================================

void demonstrate_ranges() {
    std::cout << "\n=== C++20 Ranges 演示 ===\n";
    
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // 使用ranges和views进行函数式编程
    auto even_squared = numbers 
        | std::views::filter([](int n) { return n % 2 == 0; })
        | std::views::transform([](int n) { return n * n; });
    
    std::cout << "偶数的平方: ";
    for (int value : even_squared) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    
    // 使用ranges算法
    auto result = std::ranges::find_if(numbers, [](int n) { return n > 5; });
    if (result != numbers.end()) {
        std::cout << "第一个大于5的数字: " << *result << std::endl;
    }
}

// =============================================================================
// C++20 Coroutines 演示
// =============================================================================

struct Generator {
    struct promise_type {
        int current_value;
        
        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void unhandled_exception() {}
        
        std::suspend_always yield_value(int value) {
            current_value = value;
            return {};
        }
        
        void return_void() {}
    };
    
    std::coroutine_handle<promise_type> h;
    
    Generator(std::coroutine_handle<promise_type> handle) : h(handle) {}
    
    ~Generator() { 
        if (h) h.destroy(); 
    }
    
    // 移动构造函数
    Generator(Generator&& other) noexcept : h(other.h) {
        other.h = {};
    }
    
    // 禁用复制
    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;
    
    bool next() {
        h.resume();
        return !h.done();
    }
    
    int value() {
        return h.promise().current_value;
    }
};

Generator fibonacci_generator(int count) {
    int a = 0, b = 1;
    for (int i = 0; i < count; ++i) {
        co_yield a;
        int temp = a + b;
        a = b;
        b = temp;
    }
}

void demonstrate_coroutines() {
    std::cout << "\n=== C++20 Coroutines 演示 ===\n";
    std::cout << "斐波那契数列前10项: ";
    
    auto fib = fibonacci_generator(10);
    while (fib.next()) {
        std::cout << fib.value() << " ";
    }
    std::cout << std::endl;
}

// =============================================================================
// C++20 Three-way Comparison 演示
// =============================================================================

struct Point {
    int x, y;
    
    // 使用默认的三路比较
    auto operator<=>(const Point& other) const = default;
    
    // 为了能够使用std::format，需要提供输出操作符
    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        return os << "(" << p.x << ", " << p.y << ")";
    }
};

void demonstrate_three_way_comparison() {
    std::cout << "\n=== C++20 Three-way Comparison 演示 ===\n";
    
    Point p1{1, 2};
    Point p2{1, 3};
    Point p3{1, 2};
    
    std::cout << "p1 " << p1 << ", p2 " << p2 << ", p3 " << p3 << std::endl;
    
    std::cout << "p1 == p3: " << (p1 == p3) << std::endl;
    std::cout << "p1 < p2: " << (p1 < p2) << std::endl;
    std::cout << "p1 <=> p2: ";
    
    auto result = p1 <=> p2;
    if (result < 0) {
        std::cout << "less than" << std::endl;
    } else if (result > 0) {
        std::cout << "greater than" << std::endl;
    } else {
        std::cout << "equal to" << std::endl;
    }
}

// =============================================================================
// C++20 std::format 演示 (如果编译器支持)
// =============================================================================

void demonstrate_format() {
    std::cout << "\n=== C++20 std::format 演示 ===\n";
    
    // 注意: std::format 可能还不被所有编译器完全支持
    // 这里提供一个简单的替代实现
    std::string name = "C++20";
    int version = 20;
    
    std::cout << "格式化输出: " << name << " version " << version << std::endl;
    
    // 如果编译器支持std::format，可以使用：
    // std::cout << std::format("格式化输出: {} version {}\n", name, version);
}

// =============================================================================
// 主函数
// =============================================================================

int main() {
    std::cout << "C++20 新特性演示程序\n";
    std::cout << "====================\n";
    
    // Concepts演示
    std::cout << "\n=== C++20 Concepts 演示 ===\n";
    std::cout << "add(5, 3) = " << add(5, 3) << std::endl;
    std::cout << "add(2.5, 1.7) = " << add(2.5, 1.7) << std::endl;
    
    print_value(42);
    print_value("Hello, Concepts!");
    
    // Ranges演示
    demonstrate_ranges();
    
    // Coroutines演示
    demonstrate_coroutines();
    
    // Three-way comparison演示
    demonstrate_three_way_comparison();
    
    // Format演示
    demonstrate_format();
    
    return 0;
}