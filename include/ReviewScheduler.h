#pragma once

#include <array>
#include <ctime>
#include <random>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Deck;
class Word;

class ReviewScheduler {
private:
    Deck& deck;

    array<int, 5> intervalDays = {1, 3, 7, 15, 30};

    time_t addDays(time_t time, int days) const;
public:
    explicit ReviewScheduler(Deck& deck);

    vector<Word*> dueWords(time_t now) const;
    Word* nextDue(time_t now) const;
    bool checkAnswer(const Word& word, const string& answer) const;
    void markResult(Word& word, bool correct, time_t now);
    vector<const Word*> multipleChoiceChoices(
        const Word& word, mt19937& random) const;
    const array<int, 5>& intervals() const;
    void setIntervals(const array<int, 5>& intervals);
    bool loadIntervals(const string& path);
    void saveIntervals(const string& path) const;

};
