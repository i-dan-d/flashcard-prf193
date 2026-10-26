#include "../include/Menu.h"

#include "../include/Word.h"

#include <ctime>
#include <iostream>
#include <string>

void Menu::handleReview() {
    if (deck.empty()) {
        cout << "Your vocabulary deck is empty.\n";
        return;
    }

    cout << "\n===== REVIEW =====\n"
         << "Type the meaning of each word or phrase.\n"
         << "Press Ctrl+Z then Enter to stop on Windows.\n\n";

    int reviewedCount = 0;
    int correctCount = 0;

    while (true) {
        const time_t reviewTime = time(nullptr);
        Word* word = scheduler.nextDue(reviewTime);
        if (word == nullptr) {
            break;
        }

        cout << word->getTerm() << " (" << word->getPartOfSpeech() << "): ";
        string answer;
        if (!getline(cin, answer)) {
            cout << "\nReview stopped because input ended.\n";
            return;
        }

        const bool correct = scheduler.checkAnswer(*word, answer);
        ++reviewedCount;
        if (correct) {
            ++correctCount;
            cout << "Correct!\n";
        } else {
            cout << "Not quite. Meaning: " << word->getMeaning() << '\n';
        }

        scheduler.markResult(*word, correct, reviewTime);
        cout << '\n';
    }

    if (reviewedCount == 0) {
        cout << "There are no words due for review right now.\n";
        return;
    }

    cout << "Review complete. Correct: " << correctCount
         << " / " << reviewedCount << ".\n";
}