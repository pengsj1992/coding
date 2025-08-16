#include <algorithm>
#include <bit>
#include <charconv>
#include <chrono>
#include <compare>
#include <concepts>
#include <cstdint>
#include <cstring>
#include <exception>
#include <initializer_list>
#include <iostream>
#include <numeric>
#include <ranges>
#include <source_location>
#include <span>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>
#include <cmath>
#include <stop_token>

#if defined(__has_include)
	#if __has_include(<expected>)
		#include <expected>
		#define HAS_EXPECTED 1
	#endif
	#if __has_include(<print>)
		#include <print>
		#define HAS_PRINT 1
	#endif
	#if __has_include(<version>)
		#include <version>
	#endif
#endif

using namespace std;

static void print_line(const string& s) {
#if defined(HAS_PRINT)
	std::println("{}", s);
#else
	std::cout << s << '\n';
#endif
}

// --------------- C++20 demos ---------------

// Concepts and abbreviated function templates
template <typename T>
concept Number = std::integral<T> || std::floating_point<T>;

auto add_numbers(Number auto a, Number auto b) {
	return a + b;
}

// Requires clause
template <typename T>
requires std::floating_point<T>
T hypot2(T x, T y) {
	return std::sqrt(x * x + y * y);
}

// Source location based logger
static void log_info(string_view message,
					 const std::source_location& loc = std::source_location::current()) {
	std::cout << "[" << loc.file_name() << ':' << loc.line() << "] "
			  << loc.function_name() << ": " << message << '\n';
}

// consteval function
consteval int compile_time_add(int a, int b) {
	return a + b;
}

// Using enum
enum class Color { Red, Green, Blue };

// Spaceship operator
struct Person {
	std::string name;
	int age{};
	auto operator<=>(const Person&) const = default;
};

// jthread worker
static void worker(std::stop_token st, int id) {
	using namespace std::chrono_literals;
	int ticks = 0;
	while (!st.stop_requested() && ticks < 5) {
		std::cout << "worker " << id << " tick " << ticks++ << '\n';
		std::this_thread::sleep_for(100ms);
	}
	std::cout << "worker " << id << " stopping\n";
}

static void demo_cpp20() {
	print_line("--- C++20 demos ---");

	// 1) Concepts + abbreviated templates
	{
		int ai = 3, bi = 4;
		double ad = 1.5, bd = 2.25;
		std::cout << "add_numbers(int,int) => " << add_numbers(ai, bi) << '\n';
		std::cout << "add_numbers(double,double) => " << add_numbers(ad, bd) << '\n';
	}

	// 2) Ranges: filter + transform
	{
		std::vector<int> data(20);
		std::iota(data.begin(), data.end(), 1);
		auto view = data | std::views::filter([](int x) { return x % 2 == 0; })
					   | std::views::transform([](int x) { return x * x; });
		std::vector<int> out;
		std::ranges::copy(view, std::back_inserter(out));
		std::cout << "even squares: ";
		for (int v : out) std::cout << v << ' ';
		std::cout << '\n';
	}

	// 3) std::span over an array
	{
		int raw[] = {1,2,3,4,5};
		std::span<int> s(raw);
		int sum = 0;
		for (int v : s) sum += v;
		std::cout << "sum(span) = " << sum << '\n';
	}

	// 4) consteval + constexpr
	{
		constexpr int c = compile_time_add(10, 32);
		static_assert(c == 42);
		std::cout << "compile_time_add(10,32) => " << c << '\n';
	}

	// 5) std::bit_cast
	{
		float f = 1.0f;
		std::uint32_t bits = std::bit_cast<std::uint32_t>(f);
		std::cout << "bit_cast<float->u32>(1.0f) => 0x" << std::hex << bits << std::dec << '\n';
	}

	// 6) source_location
	{
		log_info("hello from source_location logger");
	}

	// 7) <=> spaceship operator and defaulted comparisons
	{
		std::vector<Person> people{{"Ann", 30}, {"Bob", 25}, {"Cara", 40}};
		std::ranges::sort(people);
		std::cout << "sorted people by name, then age: ";
		for (auto& p : people) std::cout << '(' << p.name << ',' << p.age << ") ";
		std::cout << '\n';
	}

	// 8) using enum
	{
		using enum Color;
		Color c = Red;
		std::cout << "using enum Color => Red value: " << static_cast<int>(c) << '\n';
	}

	// 9) std::jthread + stop_token
	{
		std::jthread t1(worker, 1);
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(250ms);
		t1.request_stop();
	}
}

// --------------- C++23 demos ---------------

#if defined(__cpp_if_consteval)
	#define HAS_IF_CONSTEVAL 1
#endif

#if defined(__cpp_explicit_this_parameter)
	#define HAS_DEDUCING_THIS 1
#endif

#if defined(__cpp_lib_ranges) && defined(__cpp_lib_ranges_to_container)
	#define HAS_RANGES_TO 1
#endif

#if defined(__cpp_lib_string_contains)
	#define HAS_STRING_CONTAINS 1
#endif

// Explicit object parameter ("deducing this")
struct Accumulator {
	int sum{0};
#ifdef HAS_DEDUCING_THIS
	auto add(this Accumulator& self, int value) -> Accumulator& {
		self.sum += value;
		return self;
	}
	auto value(this const Accumulator& self) -> int { return self.sum; }
#else
	Accumulator& add(int value) {
		sum += value; return *this;
	}
	int value() const { return sum; }
#endif
};

#ifdef HAS_EXPECTED
static std::expected<int, std::string> parse_int(std::string_view sv) {
	int result = 0;
	auto first = sv.data();
	auto last = sv.data() + sv.size();
	auto [ptr, ec] = std::from_chars(first, last, result);
	if (ec == std::errc{}) return result;
	return std::unexpected(std::string("parse error"));
}
#endif

#ifdef HAS_IF_CONSTEVAL
constexpr int consteval_demo(int x) {
	if consteval {
		return x + 1; // chosen in constant evaluation
	} else {
		return x + 2; // chosen at runtime
	}
}
#endif

static void demo_cpp23() {
	print_line("--- C++23 demos ---");

	// 1) if consteval
	{
#ifdef HAS_IF_CONSTEVAL
		constexpr int a = consteval_demo(10);
		int b = consteval_demo(10);
		std::cout << "if consteval => constexpr: " << a << ", runtime: " << b << '\n';
#else
		std::cout << "if consteval not available in this compiler." << '\n';
#endif
	}

	// 2) std::expected
	{
#ifdef HAS_EXPECTED
		auto ok = parse_int("123");
		auto bad = parse_int("abc");
		std::cout << "std::expected parse '123' => ";
		if (ok) std::cout << *ok << '\n'; else std::cout << ok.error() << '\n';
		std::cout << "std::expected parse 'abc'  => ";
		if (bad) std::cout << *bad << '\n'; else std::cout << bad.error() << '\n';
#else
		std::cout << "std::expected not available (lib support missing)." << '\n';
#endif
	}

	// 3) std::string::contains
	{
		std::string s = "hello C++23";
#ifdef HAS_STRING_CONTAINS
		bool has = s.contains("C++23");
		std::cout << "string.contains('C++23') => " << std::boolalpha << has << '\n';
#else
		bool has = s.find("C++23") != std::string::npos;
		std::cout << "string.find fallback => " << std::boolalpha << has << '\n';
#endif
	}

	// 4) ranges::to to collect a view
	{
		std::vector<int> data(10);
		std::iota(data.begin(), data.end(), 1);
		auto view = data | std::views::filter([](int x) { return x % 3 == 0; });
#ifdef HAS_RANGES_TO
		auto collected = std::ranges::to<std::vector<int>>(view);
#else
		std::vector<int> collected;
		std::ranges::copy(view, std::back_inserter(collected));
#endif
		std::cout << "multiples of 3: ";
		for (int v : collected) std::cout << v << ' ';
		std::cout << '\n';
	}

	// 5) deducing this demo
	{
		Accumulator acc;
		acc.add(1).add(2).add(3);
		std::cout << "Accumulator sum => " << acc.value() << '\n';
	}

	// 6) std::print / std::println if available
	{
#ifdef HAS_PRINT
		std::println("std::println is available! {} + {} = {}", 2, 3, 2 + 3);
#else
		std::cout << "std::print not available; using std::cout instead" << '\n';
#endif
	}
}

int main() {
	std::cout << "Compiler __cplusplus == " << __cplusplus << "\n";
	#if __cplusplus >= 202002L
		demo_cpp20();
	#else
		std::cout << "C++20 not enabled by compiler." << '\n';
	#endif

	#if __cplusplus >= 202302L
		demo_cpp23();
	#else
		print_line("C++23 not fully enabled by compiler; some demos skipped.");
		demo_cpp23(); // guarded internally; will indicate missing pieces
	#endif

	return 0;
}