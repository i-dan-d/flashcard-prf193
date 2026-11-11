#include "../include/Menu.h"

#include "../include/Word.h"

#include <ctime>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

void Menu::handleReview() {
    if (deck.empty()) {
        cout << "Your vocabulary deck is empty.\n";
        return;
    }

    cout << "\n===== REVIEW =====\n"
         << "Choose the correct meaning for each word or phrase.\n\n";

    int reviewedCount = 0;
    int correctCount = 0;
    random_device seed;
    mt19937 random(seed());

    while (true) {
        const time_t reviewTime = time(nullptr);
        Word* word = scheduler.nextDue(reviewTime);
        if (word == nullptr) {
            break;
        }

        cout << word->getTerm() << " (" << word->getPartOfSpeech() << "):\n";
        const vector<const Word*> choices =
            scheduler.multipleChoiceChoices(*word, random);
        for (size_t index = 0; index < choices.size(); ++index) {
            cout << index + 1 << ". " << choices[index]->getMeaning() << '\n';
        }

        size_t selectedChoice = 0;
        while (true) {
            cout << "Choose (1-" << choices.size() << "): ";
            string line;
            if (!getline(cin, line)) {
                cout << "\nReview stopped because input ended.\n";
                return;
            }

            istringstream input(line);
            string extra;
            if ((input >> selectedChoice) && !(input >> extra) &&
                selectedChoice >= 1 && selectedChoice <= choices.size()) {
                break;
            }
            cout << "Invalid choice. Please choose a listed option.\n";
        }

        const bool correct = choices[selectedChoice - 1] == word;
        ++reviewedCount;
        if (correct) {
            ++correctCount;
            cout << "Correct!\n";
        } else {
            cout << "Not quite. Correct meaning: " << word->getMeaning()
                 << '\n';
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

void Menu::handleReviewSettings() {
    cout << "\n===== REVIEW INTERVAL SETTINGS =====\n"
         << "Enter the number of days for each review level.\n"
         << "Intervals must be positive and strictly increasing.\n";
    const array<int, 5> previousIntervals = scheduler.intervals();
    array<int, 5> updatedIntervals = previousIntervals;
    const array<int, 5>& current = previousIntervals;
    for (size_t index = 0; index < current.size(); ++index) {
        cout << "Level " << index + 1 << " (currently " << current[index]
             << " day(s)): ";
        string line;
        if (!getline(cin, line)) {
            return;
        }

        istringstream input(line);
        string extra;
        int days = 0;
        if (!(input >> days) || (input >> extra) || days <= 0) {
            cout << "Please enter a positive whole number of days.\n";
            return;
        }
        updatedIntervals[index] = days;
    }

    try {
        scheduler.setIntervals(updatedIntervals);
    } catch (const invalid_argument& error) {
        cout << "Invalid review intervals: " << error.what() << '\n';
        return;
    }

    try {
        scheduler.saveIntervals(reviewIntervalsPath);
        cout << "Review intervals saved.\n";
    } catch (const exception& error) {
        scheduler.setIntervals(previousIntervals);
        cout << "Could not save review intervals: " << error.what() << '\n';
    }
}