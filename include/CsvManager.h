#pragma once

#include <string>

class Deck;

class CsvManager {
public:
    int importFrom(const std::string& path, Deck& deck,
                   int& skippedLines) const;
    void exportTo(const std::string& path, const Deck& deck) const;

private:
    static std::string escapeField(const std::string& field);
    static bool parseLine(const std::string& line,
                          std::string& term,
                          std::string& partOfSpeech,
                          std::string& meaning);
};
