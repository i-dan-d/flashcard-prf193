#include "../include/Menu.h"

#include "../include/Word.h"

#include <iostream>
#include <sstream>

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
        int id;
        if (!readInteger("Enter the ID to edit: ", id)) {
            cout << "Please enter a valid ID.\n";
            return;
        }
        if (deck.findById(id) == nullptr) {
            cout << "No word found with that ID.\n";
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
        int id;
        if (!readInteger("Enter the ID to delete: ", id)) {
            cout << "Please enter a valid ID.\n";
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