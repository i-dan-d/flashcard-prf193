#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Word;

class Deck {
private:
    vector<unique_ptr<Word>> words;
    int nextId = 1;
public:
    bool addWord(const string& term,
                 const string& partOfSpeech,
                 const string& meaning);
    bool editWord(int id,
                  const string& newTerm,
                  const string& newPartOfSpeech,
                  const string& newMeaning);
    bool removeById(int id);

    Word* findById(int id);
    const Word* findById(int id) const;
    Word* findByTerm(const string& term);
    const Word* findByTerm(const string& term) const;

    void adopt(unique_ptr<Word> word);

    const vector<unique_ptr<Word>>& all() const;
    size_t size() const;
    bool empty() const;
    void clear();

};
