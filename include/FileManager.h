#pragma once

#include <cstdint>
#include <fstream>
#include <memory>
#include <string>

class Deck;
class Word;

class FileManager {
public:
    void save(const Deck& deck, const std::string& path) const;
    void load(Deck& deck, const std::string& path) const;
    bool exists(const std::string& path) const;

private:
    inline static constexpr std::uint32_t MAGIC = 0x464C5348;
    inline static constexpr std::uint16_t VERSION = 1;

    void writeString(std::ofstream& stream, const std::string& value) const;
    std::string readString(std::ifstream& stream) const;
    void writeWord(std::ofstream& stream, const Word& word) const;
    std::unique_ptr<Word> readWord(std::ifstream& stream) const;
};
