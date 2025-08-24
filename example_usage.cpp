#include "init.h"
#include "log.h"
#include <iostream>

// 示例：如何在代码中使用参数解析器
void demonstrate_parameter_usage() {
    // 获取全局参数解析器实例
    extern std::unique_ptr<ParameterParser> g_param_parser;
    
    if (!g_param_parser) {
        std::cerr << "参数解析器未初始化" << std::endl;
        return;
    }
    
    // 获取配置参数
    if (auto config_file = g_param_parser->getValue("config")) {
        std::cout << "配置文件: " << *config_file << std::endl;
    }
    
    // 获取日志级别
    if (auto log_level = g_param_parser->getValue("log-level")) {
        std::cout << "日志级别: " << *log_level << std::endl;
    }
    
    // 检查标志参数
    if (g_param_parser->getFlag("verbose")) {
        std::cout << "启用详细输出模式" << std::endl;
    }
    
    // 获取位置参数
    const auto& positional_args = g_param_parser->getPositionalArgs();
    if (!positional_args.empty()) {
        std::cout << "输入文件:" << std::endl;
        for (const auto& file : positional_args) {
            std::cout << "  " << file << std::endl;
        }
    }
}

// 示例：如何使用配置管理器
void demonstrate_config_usage() {
    auto& config = ConfigManager::getInstance();
    
    // 获取配置值
    if (auto log_level = config.getConfig("log_level")) {
        std::cout << "配置的日志级别: " << *log_level << std::endl;
    }
    
    if (auto sim_time = config.getConfig("simulation_time")) {
        std::cout << "模拟时间: " << *sim_time << std::endl;
    }
    
    if (auto time_step = config.getConfig("time_step")) {
        std::cout << "时间步长: " << *time_step << std::endl;
    }
    
    // 设置新的配置值
    config.setConfig("custom_setting", "custom_value");
    
    // 验证配置
    if (auto port = config.getConfig("server_port")) {
        try {
            int port_num = std::stoi(*port);
            if (port_num < 1 || port_num > 65535) {
                std::cerr << "警告: 端口号超出有效范围" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "错误: 无效的端口号格式" << std::endl;
        }
    }
}

// 示例：参数验证和错误处理
void validate_parameters() {
    extern std::unique_ptr<ParameterParser> g_param_parser;
    
    if (!g_param_parser) return;
    
    // 验证必需参数
    std::vector<std::string> required_params = {"input"};
    for (const auto& param : required_params) {
        if (!g_param_parser->getValue(param)) {
            std::cerr << "错误: 必需参数 --" << param << " 未提供" << std::endl;
            g_param_parser->showHelp();
            exit(1);
        }
    }
    
    // 验证参数值格式
    if (auto log_level = g_param_parser->getValue("log-level")) {
        std::vector<std::string> valid_levels = {"debug", "info", "warn", "error"};
        if (std::find(valid_levels.begin(), valid_levels.end(), *log_level) == valid_levels.end()) {
            std::cerr << "错误: 无效的日志级别 '" << *log_level << "'" << std::endl;
            std::cerr << "有效值: debug, info, warn, error" << std::endl;
            exit(1);
        }
    }
}

// 示例：动态添加参数
void add_custom_parameters() {
    extern std::unique_ptr<ParameterParser> g_param_parser;
    
    if (!g_param_parser) return;
    
    // 添加自定义参数
    g_param_parser->addParam({
        "output-dir", "o", 
        "输出目录路径", 
        ParameterParser::ParamType::VALUE, 
        "./output", 
        false
    });
    
    g_param_parser->addParam({
        "verbose", "V", 
        "启用详细输出", 
        ParameterParser::ParamType::FLAG, 
        "", 
        false
    });
    
    g_param_parser->addParam({
        "threads", "t", 
        "线程数量", 
        ParameterParser::ParamType::VALUE, 
        "4", 
        false
    });
}








