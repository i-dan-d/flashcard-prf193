#pragma once

#include <string>
#include <iostream>
using namespace std;

class Deck;

class CsvManager {
private:
    static string escapeField(const string& field);
    static bool parseLine(const string& line,
                          string& term,
                          string& partOfSpeech,
                          string& meaning);
public:
    int importFrom(const string& path, Deck& deck,
                   int& skippedLines) const;
    void exportTo(const string& path, const Deck& deck) const;


};
