#pragma once

#include <ctime>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Deck;
class Word;

class ReviewScheduler {
private:
    Deck& deck;

    inline static constexpr int INTERVAL_DAYS[] = {1, 3, 7, 15};

    time_t addDays(time_t time, int days) const;
public:
    explicit ReviewScheduler(Deck& deck);

    vector<Word*> dueWords(time_t now) const;
    Word* nextDue(time_t now) const;
    bool checkAnswer(const Word& word, const string& answer) const;
    void markResult(Word& word, bool correct, time_t now);


};
