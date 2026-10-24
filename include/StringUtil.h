#pragma once

#include <string>
#include <iostream>
using namespace std;
class StringUtil {
public:
    static string toLower(const string& value);
    static string trim(const string& value);
    static bool isBlank(const string& value);
    static bool equalsIgnoreCase(const string& left,
                                 const string& right);
    static bool containsIgnoreCase(const string& text,
                                   const string& key);
};
