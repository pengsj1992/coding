#include "init.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include "log.h"

// 全局参数管理器实例
std::unique_ptr<ParameterManager> g_param_manager = nullptr;

// ParameterManager 实现
ParameterManager::ParameterManager() : help_requested_(false), verbose_(false), version_requested_(false) {
    setupCLI();
}

void ParameterManager::setupCLI() {
    // 设置应用信息
    app_.name("SimX");
    app_.description("SimX - 现代化模拟器");

    // 配置文件参数
    app_.add_option("-c,--config", config_file_, "配置文件路径")->default_val("config.conf");

    // 日志级别参数
    app_.add_option("-l,--log-level", log_level_, "日志级别")->default_val("info")->check(CLI::IsMember({"debug", "info", "warn", "error"}));

    // 详细输出标志
    app_.add_flag("-v,--verbose", verbose_, "启用详细输出");

    // 版本标志 - 使用CLI11的标准方式
    app_.add_flag_callback(
        "--version",
        []() {
            std::cout << "SimX Version 1.0.0" << std::endl;
            exit(0);
        },
        "显示版本信息");

    // 位置参数 - 输入文件
    app_.add_option("files", positional_args_, "输入文件列表")->expected(0, -1);  // 0到多个文件

    // 设置帮助标志
    app_.set_help_flag("-h,--help", "显示帮助信息");
}

bool ParameterManager::parse(int argc, char* argv[]) {
    try {
        app_.parse(argc, argv);

        // 存储解析的参数
        if (!config_file_.empty()) {
            parsed_values_["config"] = config_file_;
        }
        if (!log_level_.empty()) {
            parsed_values_["log-level"] = log_level_;
        }
        if (verbose_) {
            parsed_values_["verbose"] = "true";
        }

        return true;
    } catch (const CLI::CallForHelp& e) {
        // CLI11自动处理帮助请求，直接退出
        return app_.exit(e);
    } catch (const CLI::ParseError& e) {
        return app_.exit(e);
    }
}

std::optional<std::string> ParameterManager::getValue(const std::string& name) const {
    auto it = parsed_values_.find(name);
    if (it != parsed_values_.end()) {
        return it->second;
    }
    return std::nullopt;
}

bool ParameterManager::getFlag(const std::string& name) const {
    return parsed_values_.find(name) != parsed_values_.end();
}

const std::vector<std::string>& ParameterManager::getPositionalArgs() const {
    return positional_args_;
}

void ParameterManager::showHelp() const {
    std::cout << app_.help() << std::endl;
}

bool ParameterManager::isHelpRequested() const {
    return help_requested_;
}

const std::unordered_map<std::string, std::string>& ParameterManager::getAllParsedValues() const {
    return parsed_values_;
}

// ConfigManager 实现
ConfigManager& ConfigManager::getInstance() {
    static ConfigManager instance;
    return instance;
}

bool ConfigManager::loadConfig(const std::string& config_file) {
    std::ifstream file(config_file);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        // 跳过注释和空行
        if (line.empty() || line[0] == '#') {
            continue;
        }

        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);

            // 去除首尾空格
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            config_[key] = value;
        }
    }

    return true;
}

std::optional<std::string> ConfigManager::getConfig(const std::string& key) const {
    auto it = config_.find(key);
    if (it != config_.end()) {
        return it->second;
    }
    return std::nullopt;
}

void ConfigManager::setConfig(const std::string& key, const std::string& value) {
    config_[key] = value;
}

bool ConfigManager::validateConfig() const {
    // 验证关键配置项
    std::vector<std::string> required_keys = {"log_level", "simulation_time"};

    for (const auto& key : required_keys) {
        if (config_.find(key) == config_.end()) {
            std::cerr << "警告: 缺少配置项 " << key << std::endl;
            return false;
        }
    }

    return true;
}

// 参数解析函数实现
void parse_param_in(int argc, char* argv[]) {
    g_param_manager = std::make_unique<ParameterManager>();

    if (!g_param_manager->parse(argc, argv)) {
        std::cerr << "参数解析失败" << std::endl;
        g_param_manager->showHelp();
        exit(1);
    }

    // 加载配置文件
    if (auto config_file = g_param_manager->getValue("config")) {
        if (!ConfigManager::getInstance().loadConfig(*config_file)) {
            std::cerr << "警告: 无法加载配置文件 " << *config_file << std::endl;
        }
    }

    // 设置日志级别
    if (auto log_level = g_param_manager->getValue("log-level")) {
        std::cout << "设置日志级别为: " << *log_level << std::endl;
    }

    // 显示解析的参数（调试用）
    std::cout << "解析的参数:" << std::endl;
    for (const auto& [key, value] : g_param_manager->getAllParsedValues()) {
        std::cout << "  " << key << " = " << value << std::endl;
    }

    if (!g_param_manager->getPositionalArgs().empty()) {
        std::cout << "位置参数:" << std::endl;
        for (const auto& arg : g_param_manager->getPositionalArgs()) {
            std::cout << "  " << arg << std::endl;
        }
    }
}

void init(int argc, char* argv[]) {
    parse_param_in(argc, argv);
    //    load_config();
}