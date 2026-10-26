#include "../include/ReviewScheduler.h"

#include "../include/Deck.h"
#include "../include/StringUtil.h"
#include "../include/Word.h"

#include <algorithm>
#include <limits>

ReviewScheduler::ReviewScheduler(Deck& deck) : deck(deck) {}

time_t ReviewScheduler::addDays(time_t time, int days) const {
    constexpr time_t secondsPerDay = 24 * 60 * 60;
    return time + static_cast<time_t>(days) * secondsPerDay;
}

vector<Word*> ReviewScheduler::dueWords(time_t now) const {
    vector<Word*> due;
    for (const auto& word : deck.all()) {
        if (word->getNextReviewDate() <= now) {
            due.push_back(word.get());
        }
    }

    sort(due.begin(), due.end(), [](const Word* left, const Word* right) {
        return left->getNextReviewDate() < right->getNextReviewDate();
    });
    return due;
}

Word* ReviewScheduler::nextDue(time_t now) const {
    const vector<Word*> due = dueWords(now);
    return due.empty() ? nullptr : due.front();
}

bool ReviewScheduler::checkAnswer(const Word& word,
                                  const string& answer) const {
    return StringUtil::equalsIgnoreCase(word.getMeaning(), answer);
}

void ReviewScheduler::markResult(Word& word, bool correct, time_t now) {
    if (!correct) {
        word.setReviewState(0, addDays(now, INTERVAL_DAYS[0]));
        return;
    }
    constexpr int intervalCount =
        sizeof(INTERVAL_DAYS) / sizeof(INTERVAL_DAYS[0]);

    const int currentCount = max(0, word.getReviewCount());
    const int nextCount = min(currentCount + 1, intervalCount);

    word.setReviewState(nextCount,
                        addDays(now, INTERVAL_DAYS[nextCount - 1]));
}
