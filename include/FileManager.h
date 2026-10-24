#pragma once

#include <cstdint>
#include <fstream>
#include <memory>
#include <string>
#include <iostream>
using namespace std;

class Deck;
class Word;

class FileManager {
private:
    inline static constexpr uint32_t MAGIC = 0x464C5348;
    inline static constexpr uint16_t VERSION = 1;

    void writeString(ofstream& stream, const string& value) const;
    string readString(ifstream& stream) const;
    void writeWord(ofstream& stream, const Word& word) const;
    unique_ptr<Word> readWord(ifstream& stream) const;
public:
    void save(const Deck& deck, const string& path) const;
    void load(Deck& deck, const string& path) const;
    bool exists(const string& path) const;


};
