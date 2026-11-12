#include "Deck.h"
#include "Word.h"
#include "WordLister.h"

#include <iostream>
#include <stdexcept>
#include <string>

namespace {

void require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

}  // namespace

int main() {
    Deck deck;

    require(deck.addWord("banana", "noun", "yellow fruit"),
            "Deck should accept banana");
    require(deck.addWord("apple", "noun", "red fruit"),
            "Deck should accept apple");
    require(deck.addWord("cat", "noun", "animal"),
            "Deck should accept cat");

    deck.findByTerm("banana")->setReviewState(5, 0);
    deck.findByTerm("apple")->setReviewState(2, 0);
    deck.findByTerm("cat")->setReviewState(8, 0);

    try {
        const WordLister lister(deck);
        const auto alphabetical =
            lister.sortedBy(WordLister::SortKey::Alphabet);
        require(alphabetical.size() == 3 &&
                    alphabetical[0]->getTerm() == "apple" &&
                    alphabetical[1]->getTerm() == "banana" &&
                    alphabetical[2]->getTerm() == "cat",
                "Alphabetical sort should order terms ascending");

        const auto byReviewCount =
            lister.sortedBy(WordLister::SortKey::ReviewCount);
        require(byReviewCount.size() == 3 &&
                    byReviewCount[0]->getReviewCount() == 2 &&
                    byReviewCount[1]->getReviewCount() == 5 &&
                    byReviewCount[2]->getReviewCount() == 8,
                "Review-count sort should order counts ascending");
        require(WordLister::format(*alphabetical[0]) ==
                    "apple (noun) - red fruit",
                "Formatting should include term, part of speech, and meaning");

        std::cout << "WordLister tests passed\n";
    } catch (const std::exception& error) {
        std::cerr << "WordLister test failed: " << error.what() << '\n';
        return 1;
    }

    return 0;
}