#pragma once

#include <typeinfo>
#include <string>

// 类型名获取工具类
class TypeNameUtils {
    public:
        // 获取类型名的静态方法
        template<typename T>
        static std::string getTypeName() {
            std::string name = typeid(T).name();
            return demangleTypeName(name);
        }

        // 获取对象类型名的静态方法
        template<typename T>
        static std::string getTypeName(const T& obj) {
            std::string name = typeid(obj).name();
            return demangleTypeName(name);
        }

        // 获取当前对象类型名的方法（用于在类内部使用）
        template<typename T>
        static std::string getCurrentTypeName() {
            return getTypeName<T>();
        }

    private:
        // 解析 C++ name mangling 的私有方法
        static std::string demangleTypeName(const std::string& mangled_name) {
            std::string name = mangled_name;

            // 更精确的 C++ name mangling 解析
            // 对于命名空间中的类，格式通常是 N<namespace_length><namespace_name><class_length><class_name>E
            if (name.length() > 1 && name[0] == 'N') {
                // 找到最后一个数字的位置（类名长度的开始）
                size_t lastDigitPos = name.find_last_of("0123456789");
                if (lastDigitPos != std::string::npos) {
                    // 从最后一个数字位置开始，找到类名的开始
                    size_t classNameStart = lastDigitPos + 1;
                    // 检查是否以 'E' 结尾（命名空间结束标记）
                    if (name.back() == 'E') {
                        // 提取类名（不包括结尾的 'E'）
                        name = name.substr(classNameStart, name.length() - classNameStart - 1);
                    } else {
                        // 不以 'E' 结尾，直接提取类名
                        name = name.substr(classNameStart);
                    }
                }
            } else {
                // 对于不在命名空间中的类，使用原来的简单逻辑
                size_t pos = name.find_last_of("0123456789");
                if (pos != std::string::npos) {
                    name = name.substr(pos + 1);
                }
            }

            return name;
        }
};

// 便捷的宏定义，用于获取类型名
#define GET_TYPE_NAME(T) TypeNameUtils::getTypeName<T>()
#define GET_OBJECT_TYPE_NAME(obj) TypeNameUtils::getTypeName(obj)
#define GET_CURRENT_TYPE_NAME() TypeNameUtils::getCurrentTypeName<Derived>()
