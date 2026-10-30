
#include "Deck.h"
#include "Word.h"
#include "CsvManager.h"

#include <fstream>
#include <stdexcept>
#include <vector>

string CsvManager::escapeField(const string& field) {
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
                           string& term,
                           string& partOfSpeech,
                           string& meaning) {
    vector<string> fields;
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

    // Phải có đúng 3 trường.
    if (fields.size() != 3) {
        return false;
    }

    // Gán dữ liệu cho các tham số đầu ra.
    term = fields[0];
    partOfSpeech = fields[1];
    meaning = fields[2];

    // Không chấp nhận trường rỗng.
    if (term.empty() ||
        partOfSpeech.empty() ||
        meaning.empty()) {
        return false;
    }

    return true;
}

int CsvManager::importFrom(const string& path, Deck& deck,
                           int& skippedLines) const {
    ifstream file(path);

    if (!file.is_open()) {
        throw runtime_error("Cannot open CSV file: " + path);
    }

    skippedLines = 0;
    int importedCount = 0;
    string line;

    while (getline(file, line)) {
        string term;
        string partOfSpeech;
        string meaning;

        if (!parseLine(line, term, partOfSpeech, meaning)) {
            skippedLines++;
            continue;
        }

        if (deck.addWord(term, partOfSpeech, meaning)) {
            importedCount++;
        } else {
            skippedLines++;
        }
    }

    if (file.bad()) {
        throw runtime_error("Error reading CSV file: " + path);
    }

    return importedCount;
}

void CsvManager::exportTo(const string& path, const Deck& deck) const {
    ofstream file(path);

    if (!file.is_open()) {
        throw runtime_error("Cannot open CSV file for writing: " + path);
    }

    for (const auto& wordPtr : deck.all()) {
        if (wordPtr == nullptr) {
            continue;
        }

        file << escapeField(wordPtr->getTerm()) << ", "
             << escapeField(wordPtr->getPartOfSpeech()) << ", "
             << escapeField(wordPtr->getMeaning()) << ", "
             << escapeField(wordPtr->getTypeName()) << "\n";

    }

    if (!file) {
        throw runtime_error("Error writing CSV file: " + path);
    }
}
