#include "../include/Menu.h"

#include "../include/Word.h"

#include <iostream>
#include <sstream>
#include <algorithm>

namespace {

bool readInteger(const string& prompt, int& value) {
    cout << prompt;
    string line;
    if (!getline(cin, line)) {
        return false;
    }

    istringstream input(line);
    if (!(input >> value)) {
        return false;
    }

    string extra;
    return !(input >> extra);
}

bool readWordFields(string& term, string& partOfSpeech, string& meaning) {
    cout << "Term or phrase: ";
    if (!getline(cin, term)) {
        return false;
    }

    cout << "Part of speech: ";
    if (!getline(cin, partOfSpeech)) {
        return false;
    }

    cout << "Meaning: ";
    return static_cast<bool>(getline(cin, meaning));
}

}  // namespace

void Menu::handleAddEditDelete() {
    cout << "\n===== MANAGE WORDS =====\n"
         << "1. Add a word\n"
         << "2. Edit a word\n"
         << "3. Delete a word\n"
         << "0. Back\n";

    int choice;
    if (!readInteger("Choose: ", choice)) {
        cout << "Please enter a number.\n";
        return;
    }

    if (choice == 0) {
        return;
    }

    if (choice == 1) {
        string term;
        string partOfSpeech;
        string meaning;
        if (!readWordFields(term, partOfSpeech, meaning)) {
            return;
        }

        if (deck.addWord(term, partOfSpeech, meaning)) {
            cout << "Word added.\n";
        } else {
            cout << "A word with that term already exists.\n";
        }
        return;
    }

    if (choice == 2) {
        const vector<const Word*> matches = search.menuSearch();
        if (matches.empty()) {
            return;
        }

        int id;
        if (!readInteger("Enter the ID to edit from the search results: ",
                         id)) {
            cout << "Please enter a valid ID.\n";
            return;
        }
        const auto selected = find_if(
            matches.begin(), matches.end(),
            [id](const Word* word) { return word->getId() == id; });
        if (selected == matches.end()) {
            cout << "That ID is not in the search results.\n";
            return;
        }

        string term;
        string partOfSpeech;
        string meaning;
        if (!readWordFields(term, partOfSpeech, meaning)) {
            return;
        }

        if (deck.editWord(id, term, partOfSpeech, meaning)) {
            cout << "Word updated.\n";
        } else {
            cout << "Another word already uses that term.\n";
        }
        return;
    }

    if (choice == 3) {
        const vector<const Word*> matches = search.menuSearch();
        if (matches.empty()) {
            return;
        }

        int id;
        if (!readInteger("Enter the ID to delete from the search results: ",
                         id)) {
            cout << "Please enter a valid ID.\n";
            return;
        }

        const auto selected = find_if(
            matches.begin(), matches.end(),
            [id](const Word* word) { return word->getId() == id; });
        if (selected == matches.end()) {
            cout << "That ID is not in the search results.\n";
            return;
        }

        if (deck.removeById(id)) {
            cout << "Word deleted.\n";
        } else {
            cout << "No word found with that ID.\n";
        }
        return;
    }

    cout << "Invalid choice.\n";
}