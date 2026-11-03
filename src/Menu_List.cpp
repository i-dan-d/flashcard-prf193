#ifdef _WIN32
#include <windows.h>
#endif

#include "../include/Menu.h"
#include "../include/Word.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

#ifdef _WIN32
void printUtf8(const string& text) {
    const int wideLength = MultiByteToWideChar(
        CP_UTF8, 0, text.c_str(),
        static_cast<int>(text.size()), nullptr, 0
    );

    if (wideLength <= 0) {
        return;
    }

    wstring wideText(static_cast<size_t>(wideLength), L'\0');
    MultiByteToWideChar(
        CP_UTF8, 0, text.c_str(),
        static_cast<int>(text.size()), &wideText[0], wideLength
    );

    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written = 0;
    WriteConsoleW(output, wideText.c_str(),
                  static_cast<DWORD>(wideText.size()),
                  &written, nullptr);
}
#endif

void Menu::handleList() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

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
        const string wordText =
            to_string(word->getId()) + ". " + WordLister::format(*word);

        if (key == WordLister::SortKey::ReviewCount) {
#ifdef _WIN32
            printUtf8(wordText +
                      " | Review count: " +
                      to_string(word->getReviewCount()) + "\n");
#else
            cout << wordText << " | Review count: "
                 << word->getReviewCount() << '\n';
#endif
        } else {
#ifdef _WIN32
            printUtf8(wordText + "\n");
#else
            cout << wordText << '\n';
#endif
        }
    }
}