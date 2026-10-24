#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

class Word;

class Deck {
public:
    bool addWord(const std::string& term,
                 const std::string& partOfSpeech,
                 const std::string& meaning);
    bool editWord(int id,
                  const std::string& newTerm,
                  const std::string& newPartOfSpeech,
                  const std::string& newMeaning);
    bool removeById(int id);

    Word* findById(int id);
    const Word* findById(int id) const;
    Word* findByTerm(const std::string& term);
    const Word* findByTerm(const std::string& term) const;

    void adopt(std::unique_ptr<Word> word);

    const std::vector<std::unique_ptr<Word>>& all() const;
    std::size_t size() const;
    bool empty() const;
    void clear();

private:
    std::vector<std::unique_ptr<Word>> words;
    int nextId = 1;
};
