#pragma once

#include <string>

class StringUtil {
public:
    static std::string toLower(const std::string& value);
    static std::string trim(const std::string& value);
    static bool isBlank(const std::string& value);
    static bool equalsIgnoreCase(const std::string& left,
                                 const std::string& right);
    static bool containsIgnoreCase(const std::string& text,
                                   const std::string& key);
};
