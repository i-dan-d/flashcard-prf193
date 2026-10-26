#include <iostream>
#include "../include/Deck.h"
#include "../include/Word.h"
#include "../include/WordLister.h"

using namespace std;

int main() {
    Deck deck;

    deck.addWord("banana", "noun", "chuoi");
    deck.addWord("apple", "noun", "tao");
    deck.addWord("cat", "noun", "meo");

    deck.findByTerm("banana")->setReviewState(5, 0);
    deck.findByTerm("apple")->setReviewState(2, 0);
    deck.findByTerm("cat")->setReviewState(8, 0);

    WordLister lister(deck);

    cout << "=== Sort by Alphabet ===" << endl;
    for (const Word* word : lister.sortedBy(WordLister::SortKey::Alphabet)) {
        cout << WordLister::format(*word) << endl;
    }

    cout << "\n=== Sort by Review Count ===" << endl;
    for (const Word* word : lister.sortedBy(WordLister::SortKey::ReviewCount)) {
        cout << word->getTerm()
             << " - Review count: "
             << word->getReviewCount() << endl;
    }

    return 0;
}