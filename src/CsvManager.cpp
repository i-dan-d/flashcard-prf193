
#include "Deck.h"
#include "Word.h"
#include "CsvManager.h"

#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace {

template <typename Number>
bool parseNumber(const string& value, Number& number) {
    istringstream input(value);
    input >> number;
    return input && input.peek() == char_traits<char>::eof();
}

string formatDate(time_t timestamp) {
    if (timestamp == 0) {
        return "";
    }

    tm localTime = {};
#ifdef _WIN32
    if (localtime_s(&localTime, &timestamp) != 0) {
        throw runtime_error("Could not format review date.");
    }
#else
    if (localtime_r(&timestamp, &localTime) == nullptr) {
        throw runtime_error("Could not format review date.");
    }
#endif

    ostringstream formatted;
    formatted << put_time(&localTime, "%d/%m/%Y");
    return formatted.str();
}

bool parseReviewDate(const string& value, time_t& timestamp) {
    if (value.empty()) {
        timestamp = 0;
        return true;
    }

    if (parseNumber(value, timestamp)) {
        return true;
    }

    tm localTime = {};
    char firstSeparator = '\0';
    char secondSeparator = '\0';
    istringstream input(value);
    if (!(input >> localTime.tm_mday >> firstSeparator >>
          localTime.tm_mon >> secondSeparator >> localTime.tm_year) ||
        firstSeparator != '/' || secondSeparator != '/' ||
        input.peek() != char_traits<char>::eof()) {
        return false;
    }

    if (localTime.tm_year < 1000) {
        return false;
    }
    localTime.tm_mon -= 1;
    localTime.tm_year -= 1900;
    localTime.tm_isdst = -1;

    const int expectedDay = localTime.tm_mday;
    const int expectedMonth = localTime.tm_mon;
    const int expectedYear = localTime.tm_year;
    const time_t parsed = mktime(&localTime);
    if (parsed == static_cast<time_t>(-1)) {
        return false;
    }

    tm verified = {};
#ifdef _WIN32
    if (localtime_s(&verified, &parsed) != 0) {
        return false;
    }
#else
    if (localtime_r(&parsed, &verified) == nullptr) {
        return false;
    }
#endif
    if (verified.tm_mday != expectedDay ||
        verified.tm_mon != expectedMonth ||
        verified.tm_year != expectedYear) {
        return false;
    }

    timestamp = parsed;
    return true;
}

}  // namespace

string CsvManager::escapeField(const string& field) { // xử lý dữ liệu trước khi ghi dữ liệu ra file
    bool needQuotes = false;
    string result;

    for (char c : field) {
        if (c == ',' || c == '"' || c == '\n' || c == '\r') {
            needQuotes = true;
        }

        if (c == '"') {
            result += "\"\"";
        } else {
            result += c;
        }
    }

    if (needQuotes) {
        result = "\"" + result + "\"";
    }

    return result;
}

bool CsvManager::parseLine(const string& line,
                           vector<string>& fields) {
    fields.clear();
    string field;
    bool inQuotes = false;
    bool afterQuote = false;

    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];

        if (inQuotes) {
            if (c == '"') {
                if (i + 1 < line.size() && line[i + 1] == '"') {
                    field += '"';
                    ++i;
                } else {
                    inQuotes = false;
                    afterQuote = true;
                }
            } else {
                field += c;
            }
        } else {
            if (afterQuote) {
                if (c == ',') {
                    fields.push_back(field);
                    field.clear();
                    afterQuote = false;
                } else {
                    return false;
                }
            } else {
                if (c == ',') {
                    fields.push_back(field);
                    field.clear();
                } else if (c == '"') {
                    if (!field.empty()) {
                        return false;
                    }

                    inQuotes = true;
                } else {
                    field += c;
                }
            }
        }
    }

    // Ngoặc kép chưa được đóng.
    if (inQuotes) {
        return false;
    }

    // Lưu trường cuối cùng.
    fields.push_back(field);

    if (fields.size() != 3 && fields.size() != 5) {
        return false;
    }
    if (fields[0].empty() || fields[1].empty() || fields[2].empty()) {
        return false;
    }

    if (fields.size() == 5) {
        int reviewCount;
        time_t nextReviewDate;
        if (!parseNumber(fields[3], reviewCount) || reviewCount < 0 ||
            !parseReviewDate(fields[4], nextReviewDate)) {
            return false;
        }
    }

    return true;
}

int CsvManager::importFrom(const string& path, Deck& deck,
                           int& skippedLines) const {
    ifstream file(path); // mở file

    if (!file.is_open()) {
        throw runtime_error("Cannot open CSV file: " + path);
    }
    // khởi tạo bộ đếm
    skippedLines = 0;
    int importedCount = 0;
    string line;
    bool firstLine = true;
    while (getline(file, line)) {
        if (firstLine && line.size() >= 3 &&
            static_cast<unsigned char>(line[0]) == 0xEF &&
            static_cast<unsigned char>(line[1]) == 0xBB &&
            static_cast<unsigned char>(line[2]) == 0xBF) {
            line.erase(0, 3);
        }
        firstLine = false;

        if (line == "term,partOfSpeech,meaning,reviewCount,nextReviewDate") {
            continue;
        }

        vector<string> fields;
        if (!parseLine(line, fields)) {
            skippedLines++;
            continue;
        }

        int reviewCount = 0;
        time_t nextReviewDate = 0;
        if (fields.size() == 5 &&
            (!parseNumber(fields[3], reviewCount) || reviewCount < 0 ||
             !parseReviewDate(fields[4], nextReviewDate))) {
            skippedLines++;
            continue;
        }

        if (deck.addWord(fields[0], fields[1], fields[2])) {
            if (fields.size() == 5) {
                deck.findByTerm(fields[0])->setReviewState(
                    reviewCount, nextReviewDate);
            }
            importedCount++;
        } else {
            skippedLines++;
        }
    }
    // kiểm tra lỗi đọc file
    if (file.bad()) {
        throw runtime_error("Error reading CSV file: " + path);
    }
    // trả số lượng nhập thành công
    return importedCount;
}

void CsvManager::exportTo(const string& path, const Deck& deck) const {
    ofstream file(path); // mở file ghi 

    if (!file.is_open()) {
        throw runtime_error("Cannot open CSV file for writing: " + path);
    }
    file << "term,partOfSpeech,meaning,reviewCount,nextReviewDate\n";
    for (const auto& wordPtr : deck.all()) {
        if (wordPtr == nullptr) {
            continue;
        }

        file << escapeField(wordPtr->getTerm()) << ','
             << escapeField(wordPtr->getPartOfSpeech()) << ','
             << escapeField(wordPtr->getMeaning()) << ','
             << wordPtr->getReviewCount() << ','
             << formatDate(wordPtr->getNextReviewDate()) << '\n';

    }
    // kiểu tra lỗi ghi 
    if (!file) {
        throw runtime_error("Error writing CSV file: " + path);
    }
}
