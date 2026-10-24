#pragma once

#include <string>
#include <vector>

class Deck;
class Word;

class SearchService {
public:
    explicit SearchService(const Deck& deck);

    std::vector<const Word*> search(const std::string& keyword) const;

private:
    const Deck& deck;

    bool matches(const Word& word, const std::string& keyLower) const;
};
