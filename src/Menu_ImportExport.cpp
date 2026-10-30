
#include "Menu.h"

#include <iostream>
#include <limits>
#include <string>
#include <stdexcept>

using namespace std;

void Menu::handleImportExport() {
    int choice;

    cout << "\n===== IMPORT / EXPORT =====\n";
    cout << "1. Import words from CSV\n";
    cout << "2. Export words to CSV\n";
    cout << "0. Back\n";
    cout << "Choose: ";

    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input.\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == 0) {
        return;
    }

    if (choice != 1 && choice != 2) {
        cout << "Invalid choice.\n";
        return;
    }

    string path;
    cout << "Enter CSV file path: ";
    getline(cin, path);

    if (path.empty()) {
        cout << "File path cannot be empty.\n";
        return;
    }

    try {
        if (choice == 1) {
            int skippedLines = 0;
            int importedCount = csv.importFrom(path, deck, skippedLines);

            cout << "Imported successfully: "
                 << importedCount << " word(s).\n";
            cout << "Skipped lines: " << skippedLines << "\n";
        } else {
            csv.exportTo(path, deck);
            cout << "Export completed successfully.\n";
        }
    } catch (const exception& e) {
        cout << "Error: " << e.what() << "\n";
    }
}
