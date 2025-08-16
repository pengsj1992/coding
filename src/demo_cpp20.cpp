#include <algorithm>
#include <chrono>
#include <compare>
#include <concepts>
#include <iostream>
#include <ranges>
#include <source_location>
#include <span>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

// Concepts

template <typename T>
concept Integer = std::integral<T>;

Integer auto add(Integer auto a, Integer auto b) {
    return a + b;
}

// Ranges

void demo_ranges() {
    std::vector<int> data{1,2,3,4,5,6,7,8,9,10};
    auto even_squares = data
        | std::views::filter([](int x) { return x % 2 == 0; })
        | std::views::transform([](int x) { return x * x; });

    std::cout << "C++20 Ranges (even squares): ";
    for (int v : even_squares) {
        std::cout << v << ' ';
    }
    std::cout << '\n';
}

// consteval / constinit

consteval int square_consteval(int x) {
    return x * x;
}

constinit int global_counter = 0;

// <=> three-way comparison

struct Point {
    int x{};
    int y{};
    auto operator<=>(const Point&) const = default;
};

// span

int sum_span(std::span<const int> values) {
    int sum = 0;
    for (int v : values) {
        sum += v;
    }
    return sum;
}

// source_location

void log(std::string_view message, const std::source_location& loc = std::source_location::current()) {
    std::cout << loc.file_name() << ':' << loc.line() << " (" << loc.function_name() << ") -> " << message << '\n';
}

// designated initializers (aggregate)

struct Person {
    std::string name;
    int age;
};

// jthread + stop_token

void worker(std::stop_token st) {
    using namespace std::chrono_literals;
    int tick = 0;
    while (!st.stop_requested() && tick < 5) {
        std::this_thread::sleep_for(100ms);
        std::cout << "jthread working... tick=" << tick++ << '\n';
    }
    std::cout << "jthread exiting" << '\n';
}

int main() {
    std::cout << "===== C++20 Demo =====" << '\n';

    // Concepts
    int sum = add(10, 32);
    std::cout << "Concepts add(10,32) = " << sum << '\n';

    // Ranges
    demo_ranges();

    // consteval / constinit
    constexpr int s = square_consteval(7);
    std::cout << "consteval square_consteval(7) = " << s << '\n';
    global_counter += 1;
    std::cout << "constinit global_counter = " << global_counter << '\n';

    // <=> three-way comparison
    Point a{.x = 1, .y = 2};
    Point b{.x = 1, .y = 3};
    std::cout << "(a < b) = " << (a < b) << ", (a == a) = " << (a == a) << '\n';

    // span
    std::vector<int> nums{1,2,3,4,5};
    std::cout << "sum_span([1..5]) = " << sum_span(nums) << '\n';

    // source_location
    log("hello from source_location");

    // designated initializers
    Person p{.name = "Alice", .age = 30};
    std::cout << "Person{" << p.name << ", " << p.age << "}" << '\n';

    // jthread
    std::jthread t(worker);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    t.request_stop();

    return 0;
}