#include "../include/FileManager.h"

#include "../include/Deck.h"
#include "../include/PhraseWord.h"
#include "../include/VocabularyWord.h"
#include "../include/Word.h"

#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>

namespace {

const vector<string> STORAGE_HEADER = {
    "id", "type", "term", "partOfSpeech", "meaning", "reviewCount",
    "nextReviewDate"};

string escapeField(const string& field) {
    bool needsQuotes = false;
    string escaped;
    for (char character : field) {
        if (character == ',' || character == '"' || character == '\n' ||
            character == '\r') {
            needsQuotes = true;
        }
        if (character == '"') {
            escaped += "\"\"";
        } else {
            escaped += character;
        }
    }

    return needsQuotes ? "\"" + escaped + "\"" : escaped;
}

vector<vector<string>> parseCsv(const string& contents) {
    vector<vector<string>> records;
    vector<string> fields;
    string field;
    bool inQuotes = false;
    bool afterQuote = false;

    for (size_t index = 0; index < contents.size(); ++index) {
        const char character = contents[index];
        if (inQuotes) {
            if (character == '"') {
                if (index + 1 < contents.size() &&
                    contents[index + 1] == '"') {
                    field += '"';
                    ++index;
                } else {
                    inQuotes = false;
                    afterQuote = true;
                }
            } else {
                field += character;
            }
            continue;
        }

        if (afterQuote) {
            if (character == ',') {
                fields.push_back(field);
                field.clear();
                afterQuote = false;
            } else if (character == '\n' || character == '\r') {
                fields.push_back(field);
                records.push_back(fields);
                fields.clear();
                field.clear();
                afterQuote = false;
                if (character == '\r' && index + 1 < contents.size() &&
                    contents[index + 1] == '\n') {
                    ++index;
                }
            } else {
                throw runtime_error("Invalid characters after quoted CSV field.");
            }
            continue;
        }

        if (character == ',') {
            fields.push_back(field);
            field.clear();
        } else if (character == '\n' || character == '\r') {
            fields.push_back(field);
            records.push_back(fields);
            fields.clear();
            field.clear();
            if (character == '\r' && index + 1 < contents.size() &&
                contents[index + 1] == '\n') {
                ++index;
            }
        } else if (character == '"') {
            if (!field.empty()) {
                throw runtime_error("Unexpected quote in CSV field.");
            }
            inQuotes = true;
        } else {
            field += character;
        }
    }

    if (inQuotes) {
        throw runtime_error("Unterminated quoted CSV field.");
    }
    if (afterQuote || !fields.empty() || !field.empty()) {
        fields.push_back(field);
        records.push_back(fields);
    }

    return records;
}

template <typename Number>
Number parseNumber(const string& value, const string& fieldName) {
    istringstream input(value);
    Number number;
    input >> number;
    if (!input || input.peek() != char_traits<char>::eof()) {
        throw runtime_error("Invalid " + fieldName + " in vocabulary CSV.");
    }
    return number;
}

string formatReviewDate(time_t timestamp) {
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

time_t parseReviewDate(const string& value) {
    if (value.empty()) {
        return 0;
    }

    istringstream timestampInput(value);
    time_t timestamp;
    timestampInput >> timestamp;
    if (timestampInput && timestampInput.peek() == char_traits<char>::eof()) {
        return timestamp;
    }

    tm localTime = {};
    char firstSeparator = '\0';
    char secondSeparator = '\0';
    istringstream dateInput(value);
    if (!(dateInput >> localTime.tm_mday >> firstSeparator >>
          localTime.tm_mon >> secondSeparator >> localTime.tm_year) ||
        firstSeparator != '/' || secondSeparator != '/' ||
        dateInput.peek() != char_traits<char>::eof() ||
        localTime.tm_year < 1000) {
        throw runtime_error("Invalid next review date in vocabulary CSV.");
    }

    localTime.tm_mon -= 1;
    localTime.tm_year -= 1900;
    localTime.tm_isdst = -1;
    const int expectedDay = localTime.tm_mday;
    const int expectedMonth = localTime.tm_mon;
    const int expectedYear = localTime.tm_year;
    timestamp = mktime(&localTime);
    if (timestamp == static_cast<time_t>(-1)) {
        throw runtime_error("Invalid next review date in vocabulary CSV.");
    }

    tm verified = {};
#ifdef _WIN32
    if (localtime_s(&verified, &timestamp) != 0) {
        throw runtime_error("Invalid next review date in vocabulary CSV.");
    }
#else
    if (localtime_r(&timestamp, &verified) == nullptr) {
        throw runtime_error("Invalid next review date in vocabulary CSV.");
    }
#endif
    if (verified.tm_mday != expectedDay || verified.tm_mon != expectedMonth ||
        verified.tm_year != expectedYear) {
        throw runtime_error("Invalid next review date in vocabulary CSV.");
    }

    return timestamp;
}

void requireFieldCount(const vector<string>& fields, size_t expected) {
    if (fields.size() != expected) {
        throw runtime_error("Invalid number of fields in vocabulary CSV.");
    }
}

}  // namespace

void FileManager::save(const Deck& deck, const string& path) const {
    ofstream stream(path);
    if (!stream) {
        throw runtime_error("Could not open file for writing: " + path);
    }

    for (size_t index = 0; index < STORAGE_HEADER.size(); ++index) {
        if (index != 0) {
            stream << ',';
        }
        stream << STORAGE_HEADER[index];
    }
    stream << '\n';

    for (const auto& word : deck.all()) {
        if (word == nullptr) {
            continue;
        }
        stream << word->getId() << ','
               << escapeField(word->getTypeName()) << ','
               << escapeField(word->getTerm()) << ','
               << escapeField(word->getPartOfSpeech()) << ','
               << escapeField(word->getMeaning()) << ','
               << word->getReviewCount() << ','
               << formatReviewDate(word->getNextReviewDate()) << '\n';
    }

    if (!stream) {
        throw runtime_error("Could not save vocabulary CSV file: " + path);
    }
}

void FileManager::load(Deck& deck, const string& path) const {
    ifstream stream(path);
    if (!stream) {
        throw runtime_error("Could not open file for reading: " + path);
    }

    const string contents((istreambuf_iterator<char>(stream)),
                          istreambuf_iterator<char>());
    if (stream.bad()) {
        throw runtime_error("Could not read vocabulary CSV file: " + path);
    }

    vector<vector<string>> records = parseCsv(contents);
    if (!records.empty() && records.front().size() == 1 &&
        records.front().front().empty()) {
        records.erase(records.begin());
    }

    bool hasStorageHeader = false;
    if (!records.empty() && records.front() == STORAGE_HEADER) {
        hasStorageHeader = true;
        records.erase(records.begin());
    }

    Deck loadedDeck;
    for (const auto& fields : records) {
        if (fields.size() == 1 && fields.front().empty()) {
            continue;
        }

        if (!hasStorageHeader) {
            requireFieldCount(fields, 3);
            if (!loadedDeck.addWord(fields[0], fields[1], fields[2])) {
                throw runtime_error("Duplicate term in vocabulary CSV.");
            }
            continue;
        }

        requireFieldCount(fields, STORAGE_HEADER.size());
        const int id = parseNumber<int>(fields[0], "word ID");
        const int reviewCount = parseNumber<int>(fields[5], "review count");
        const time_t nextReviewDate = parseReviewDate(fields[6]);
        if (id < 1 || reviewCount < 0) {
            throw runtime_error("Invalid word ID or review count in vocabulary CSV.");
        }

        unique_ptr<Word> word;
        if (fields[1] == "Phrase") {
            word = make_unique<PhraseWord>(id, fields[2], fields[3], fields[4]);
        } else if (fields[1] == "Vocabulary") {
            word = make_unique<VocabularyWord>(
                id, fields[2], fields[3], fields[4]);
        } else {
            throw runtime_error("Unknown word type in vocabulary CSV.");
        }
        word->setReviewState(reviewCount, nextReviewDate);
        loadedDeck.adopt(std::move(word));
    }

    deck = std::move(loadedDeck);
}

bool FileManager::exists(const string& path) const {
    ifstream stream(path);
    return stream.good();
}
