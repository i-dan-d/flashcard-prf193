#pragma once

#include <string>
#include <vector>

class Deck;
class Word;

class WordLister {
public:
    enum class SortKey {
        Alphabet,
        ReviewCount
    };

    explicit WordLister(const Deck& deck);

    std::vector<const Word*> sortedBy(SortKey key) const;
    static std::string format(const Word& word);

private:
    const Deck& deck;

    static bool lessByAlphabet(const Word* left, const Word* right);
    static bool lessByReviewCount(const Word* left, const Word* right);
};
