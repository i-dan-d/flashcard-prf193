
#include "Deck.h"
#include "Word.h"
#include "CsvManager.h"

#include <fstream>
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
            !parseNumber(fields[4], nextReviewDate)) {
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
    while (getline(file, line)) {
        vector<string> fields;
        if (!parseLine(line, fields)) {
            skippedLines++;
            continue;
        }

        int reviewCount = 0;
        time_t nextReviewDate = 0;
        if (fields.size() == 5 &&
            (!parseNumber(fields[3], reviewCount) || reviewCount < 0 ||
             !parseNumber(fields[4], nextReviewDate))) {
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
    // duyệt toàn bộ word trong deck
    for (const auto& wordPtr : deck.all()) {
        if (wordPtr == nullptr) {
            continue;
        }

        file << escapeField(wordPtr->getTerm()) << ','
             << escapeField(wordPtr->getPartOfSpeech()) << ','
             << escapeField(wordPtr->getMeaning()) << ','
             << wordPtr->getReviewCount() << ','
             << wordPtr->getNextReviewDate() << '\n';

    }
    // kiểu tra lỗi ghi 
    if (!file) {
        throw runtime_error("Error writing CSV file: " + path);
    }
}
