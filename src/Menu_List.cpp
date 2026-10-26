#include "../include/Menu.h"
#include "../include/Word.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

void Menu::handleList() {
    if (deck.empty()) {
        cout << "The vocabulary deck is empty.\n";
        return;
    }

    cout << "\n===== LIST ALL WORDS =====\n"
         << "1. Sort by alphabet\n"
         << "2. Sort by review count\n"
         << "Choose: ";

    string input;
    if (!getline(cin, input)) {
        return;
    }

    WordLister::SortKey key;
    if (input == "1") {
        key = WordLister::SortKey::Alphabet;
    } else if (input == "2") {
        key = WordLister::SortKey::ReviewCount;
    }
    else {
        cout << "Invalid choice.\n";
        return;
    }

    vector<const Word*> words = lister.sortedBy(key);

    cout << "\n===== WORD LIST =====\n";

    for (const Word* word : words) {
        cout << word->getId() << ". "
             << WordLister::format(*word);
        if (key == WordLister::SortKey::ReviewCount) {
            cout << " | Review count: " << word->getReviewCount();
        }
        cout << '\n';
    }
}