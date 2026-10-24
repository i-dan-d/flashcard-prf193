#include "../include/StringUtil.h"

#include <algorithm>
#include <cctype>

string StringUtil::toLower(const string& value) {
    string result = value;
    transform(result.begin(), result.end(), result.begin(),
              [](unsigned char character) {
                  return static_cast<char>(tolower(character));
              });
    return result;
}

string StringUtil::trim(const string& value) {
    const auto isSpace = [](unsigned char character) {
        return isspace(character) != 0;
    };

    const auto first = find_if_not(value.begin(), value.end(), isSpace);
    if (first == value.end()) {
        return "";
    }

    const auto last = find_if_not(value.rbegin(), value.rend(), isSpace).base();
    return string(first, last);
}

bool StringUtil::isBlank(const string& value) {
    return all_of(value.begin(), value.end(),
                  [](unsigned char character) {
                      return isspace(character) != 0;
                  });
}

bool StringUtil::equalsIgnoreCase(const string& left, const string& right) {
    return toLower(left) == toLower(right);
}

bool StringUtil::containsIgnoreCase(const string& text, const string& key) {
    return toLower(text).find(toLower(key)) != string::npos;
}
