    #pragma once

    #include <string>
    #include <vector>
    #include <iostream>
    using namespace std;
    class Deck;

    class Word;

    class WordLister {
    private:
        const Deck& deck;

        static bool lessByAlphabet(const Word* left, const Word* right);
        static bool lessByReviewCount(const Word* left, const Word* right);
    public:
        enum class SortKey {
            Alphabet,
            ReviewCount
        };

        explicit WordLister(const Deck& deck);

        vector<const Word*> sortedBy(SortKey key) const;
        static string format(const Word& word);


    };
