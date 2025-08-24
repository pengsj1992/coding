#pragma once

#include <CLI/CLI.hpp>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

// 现代化参数管理器 - 基于CLI11
class ModernParameterManager {
   public:
    ModernParameterManager();
    ~ModernParameterManager() = default;

    // 初始化CLI应用
    void setupCLI();

    // 解析参数
    bool parse(int argc, char* argv[]);

    // 获取参数值
    std::optional<std::string> getValue(const std::string& name) const;
    bool getFlag(const std::string& name) const;

    // 获取位置参数
    const std::vector<std::string>& getPositionalArgs() const;

    // 显示帮助信息
    void showHelp() const;

    // 检查是否请求帮助
    bool isHelpRequested() const;

    // 获取CLI应用实例（用于高级用法）
    CLI::App* getCLIApp() { return &app_; }

   private:
    CLI::App app_;
    std::unordered_map<std::string, std::string> parsed_values_;
    std::vector<std::string> positional_args_;
    bool help_requested_;

    // 参数存储
    std::string config_file_;
    std::string log_level_;
    bool verbose_;
    bool version_requested_;
};

// 配置管理器（保持原有设计）
class ConfigManager {
   public:
    static ConfigManager& getInstance();

    // 加载配置文件
    bool loadConfig(const std::string& config_file);

    // 获取配置值
    std::optional<std::string> getConfig(const std::string& key) const;

    // 设置配置值
    void setConfig(const std::string& key, const std::string& value);

    // 验证配置
    bool validateConfig() const;

   private:
    ConfigManager() = default;
    std::unordered_map<std::string, std::string> config_;
};

// 全局参数管理器实例
extern std::unique_ptr<ModernParameterManager> g_param_manager;

// 现代化初始化函数
void init_modern(int argc, char* argv[]);

// 参数解析函数
void parse_param_modern(int argc, char* argv[]);
