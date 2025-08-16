#include <algorithm>
#include <bit>
#include <expected>
#include <functional>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include <memory>

// std::expected
std::expected<int, std::string> parse_int(std::string_view text) {
    int value = 0;
    bool ok = !text.empty();
    for (char c : text) {
        if (c < '0' || c > '9') { ok = false; break; }
        value = value * 10 + (c - '0');
    }
    if (ok) return value;
    return std::unexpected("not a positive integer");
}

// if consteval
constexpr int consteval_or_runtime() {
    if consteval {
        return 1; // evaluated at compile-time
    } else {
        return 2; // evaluated at run-time
    }
}

// explicit object parameter (deducing this)
struct Accumulator {
    int value{0};

    auto add(this Accumulator& self, int delta) -> Accumulator& {
        self.value += delta;
        return self;
    }

    auto get(this const Accumulator& self) -> int { return self.value; }
};

int main() {
    std::cout << "===== C++23 Demo =====" << '\n';

    // std::expected
    for (std::string_view s : {"123", "42x"}) {
        auto r = parse_int(s);
        if (r) {
            std::cout << "parse_int('" << s << "') = " << *r << '\n';
        } else {
            std::cout << "parse_int('" << s << "') error: " << r.error() << '\n';
        }
    }

    // if consteval
    constexpr int x = consteval_or_runtime();
    int y = consteval_or_runtime();
    std::cout << "if consteval: constexpr call -> " << x << ", runtime call -> " << y << '\n';

    // explicit object parameter (deducing this)
    Accumulator acc{};
    acc.add(5).add(7);
    std::cout << "Accumulator value = " << acc.get() << '\n';

    // ranges::to
    auto vec = std::views::iota(1, 6)
             | std::views::transform([](int v) { return v * v; })
             | std::ranges::to<std::vector>();
    std::cout << "ranges::to -> ";
    for (int v : vec) std::cout << v << ' ';
    std::cout << '\n';

    // string::contains
    std::string text = "hello world";
    std::cout << "string::contains('ell') -> " << text.contains("ell") << '\n';

    // move_only_function
    std::move_only_function<int(int)> f = [capt = std::make_unique<int>(3)](int x) {
        return x + *capt;
    };
    std::cout << "move_only_function(5) -> " << f(5) << '\n';

    // views::zip
    std::vector<int> a{1,2,3};
    std::vector<int> b{10,20,30,40};
    std::cout << "zip -> ";
    for (auto [i, j] : std::views::zip(a, b)) {
        std::cout << '(' << i << '+' << j << '=' << (i + j) << ") ";
    }
    std::cout << '\n';

    // byteswap
    std::uint32_t u = 0x12345678u;
    std::uint32_t s = std::byteswap(u);
    std::cout << std::hex << "byteswap(0x12345678) -> 0x" << s << std::dec << '\n';

    return 0;
}