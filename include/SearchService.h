#pragma once

#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Deck;
class Word;

class SearchService {
private:
    const Deck& deck;

    bool matches(const Word& word, const string& keyLower) const;
public:
    explicit SearchService(const Deck& deck);

    vector<const Word*> search(const string& keyword) const;
    void menuSearch(const Deck& deck) const;


};
