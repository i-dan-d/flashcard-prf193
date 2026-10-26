#include "../include/Menu.h"

#include <exception>
#include <iostream>
#include <sstream>

Menu::Menu() : lister(deck), scheduler(deck) {}

int Menu::readChoice() const {
    cout << "\n===== FLASHCARD MENU =====\n"
         << "1. Add, edit, or delete words\n"
         << "2. Search (not implemented yet)\n"
         << "3. List words\n"
         << "4. Review due words\n"
         << "5. Import/export CSV (not implemented yet)\n"
         << "0. Save and exit\n"
         << "Choose: ";

    string line;
    if (!getline(cin, line)) {
        return -2;
    }

    istringstream input(line);
    int choice;
    if (!(input >> choice)) {
        return -1;
    }
    string extra;
    if (input >> extra) {
        return -1;
    }
    return choice;
}

void Menu::loadOnStart() {
    if (!fileManager.exists(dataPath)) {
        cout << "No saved deck found. Starting with an empty deck.\n";
        return;
    }

    try {
        fileManager.load(deck, dataPath);
        cout << "Loaded " << deck.size() << " word(s).\n";
    } catch (const exception& error) {
        cout << "Could not load saved deck: " << error.what() << '\n';
        cout << "Starting with an empty deck.\n";
    }
}

void Menu::saveOnExit() {
    try {
        fileManager.save(deck, dataPath);
        cout << "Deck saved.\n";
    } catch (const exception& error) {
        cout << "Could not save deck: " << error.what() << '\n';
    }
}

void Menu::handleSearch() {
    cout << "Search is not available yet.\n";
}

void Menu::handleImportExport() {
    cout << "CSV import/export is not available yet.\n";
}

void Menu::run() {
    loadOnStart();

    while (true) {
        const int choice = readChoice();
        if (choice == -2 || choice == 0) {
            saveOnExit();
            return;
        }

        switch (choice) {
            case 1:
                handleAddEditDelete();
                break;
            case 2:
                handleSearch();
                break;
            case 3:
                handleList();
                break;
            case 4:
                handleReview();
                break;
            case 5:
                handleImportExport();
                break;
            default:
                cout << "Invalid choice. Enter a number from 0 to 5.\n";
                break;
        }
    }
}