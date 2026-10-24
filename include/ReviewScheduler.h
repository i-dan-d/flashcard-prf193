#pragma once

#include <ctime>
#include <string>
#include <vector>

class Deck;
class Word;

class ReviewScheduler {
public:
    explicit ReviewScheduler(Deck& deck);

    std::vector<Word*> dueWords(std::time_t now) const;
    Word* nextDue(std::time_t now) const;
    bool checkAnswer(const Word& word, const std::string& answer) const;
    void markResult(Word& word, bool correct, std::time_t now);

private:
    Deck& deck;

    inline static constexpr int INTERVAL_DAYS[] = {1, 3, 7, 15};

    std::time_t addDays(std::time_t time, int days) const;
};
