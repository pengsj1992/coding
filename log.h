#pragma once

#include <fmt/format.h>
#include <spdlog/logger.h>
#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <source_location>
#include <string>
#include <unordered_map>
#include "utils/type_name.h"

// 编译期文件名提取函数, 经测试确认为编译时计算
constexpr std::string_view constexpr_get_filename(std::string_view path) {
    auto last_slash_pos = path.find_last_of('/');
    if (last_slash_pos != std::string_view::npos) {
        return path.substr(last_slash_pos + 1);
    }
    return path;
}

class LogManager {
   public:
    static LogManager& getInstance() {
        static LogManager instance;
        return instance;
    }

    static std::shared_ptr<spdlog::logger> getLogger(const std::string& module_name) {
        auto it = loggers_.find(module_name);
        if (it != loggers_.end()) {
            return it->second;
        }

#if DEBUG  // Debug模式下，同时输出到控制台和文件, 日志级别设置到 debug
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/" + module_name + ".log");
        auto logger = std::make_shared<spdlog::logger>(module_name, spdlog::sinks_init_list{console_sink, file_sink});
        logger->set_level(spdlog::level::debug);
#else  // Release 模式仅输出到文件, 日志级别设置到 warn
        auto logger = spdlog::basic_logger_mt(module_name, "logs/" + module_name + ".log");
        logger->set_level(spdlog::level::warn);
#endif

        logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%n] %v");
        loggers_[module_name] = logger;
        return logger;
    }

   private:
    LogManager() = default;
    static inline std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> loggers_;
};

// 可日志化的基类 - 使用CRTP模式
template <typename Derived>
class Loggable {
   public:
    Loggable() : logger_(LogManager::getLogger(TypeNameUtils::getTypeName<Derived>())) {}

    template <typename... Args>
    void trace(fmt::format_string<Args...> fmt, Args&&... args) {
        logger_->trace(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void debug(fmt::format_string<Args...> fmt, Args&&... args) {
        logger_->debug(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void info(fmt::format_string<Args...> fmt, Args&&... args) {
        logger_->info(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void warn(fmt::format_string<Args...> fmt, Args&&... args, const std::source_location& location = std::source_location::current()) {
        logger_->warn(fmt::format("[{}:{}] {}", constexpr_get_filename(location.file_name()), location.line(), fmt), std::forward<Args>(args)...);
    }

    template <typename... Args>
    void error(fmt::format_string<Args...> fmt, Args&&... args, const std::source_location& location = std::source_location::current()) {
        logger_->error(fmt::format("[{}:{}] {}", constexpr_get_filename(location.file_name()), location.line(), fmt), std::forward<Args>(args)...);
    }

    template <typename... Args>
    void critical(fmt::format_string<Args...> fmt, Args&&... args, const std::source_location& location = std::source_location::current()) {
        logger_->critical(fmt::format("[{}:{}] {}", constexpr_get_filename(location.file_name()), location.line(), fmt), std::forward<Args>(args)...);
    }

    template <typename... Args>
    void fatal(fmt::format_string<Args...> fmt, Args&&... args) {
        logger_->critical(fmt, std::forward<Args>(args)...);
    }

   private:
    std::shared_ptr<spdlog::logger> logger_;
};