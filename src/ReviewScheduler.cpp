#include "../include/ReviewScheduler.h"

#include "../include/Deck.h"
#include "../include/StringUtil.h"
#include "../include/Word.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <utility>

ReviewScheduler::ReviewScheduler(Deck& deck) : deck(deck) {}

time_t ReviewScheduler::addDays(time_t time, int days) const {
    constexpr time_t secondsPerDay = 24 * 60 * 60;
    const time_t maximum = numeric_limits<time_t>::max();
    if (days > maximum / secondsPerDay) {
        throw overflow_error("Review interval is too large.");
    }

    const time_t seconds = static_cast<time_t>(days) * secondsPerDay;
    if (time > maximum - seconds) {
        throw overflow_error("Next review date is out of range.");
    }
    return time + seconds;
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

vector<const Word*> ReviewScheduler::multipleChoiceChoices(
    const Word& word, mt19937& random) const {
    vector<const Word*> distractors;
    for (const auto& candidate : deck.all()) {
        if (candidate.get() == &word ||
            StringUtil::equalsIgnoreCase(candidate->getMeaning(),
                                         word.getMeaning())) {
            continue;
        }
        distractors.push_back(candidate.get());
    }

    shuffle(distractors.begin(), distractors.end(), random);
    if (distractors.size() > 3) {
        distractors.resize(3);
    }

    vector<const Word*> choices = std::move(distractors);
    choices.push_back(&word);
    shuffle(choices.begin(), choices.end(), random);
    return choices;
}

const array<int, 5>& ReviewScheduler::intervals() const {
    return intervalDays;
}

void ReviewScheduler::setIntervals(const array<int, 5>& intervals) {
    for (size_t index = 0; index < intervals.size(); ++index) {
        if (intervals[index] <= 0 ||
            (index > 0 && intervals[index] <= intervals[index - 1])) {
            throw invalid_argument(
                "Review intervals must be positive and strictly increasing.");
        }
    }
    intervalDays = intervals;
}

bool ReviewScheduler::loadIntervals(const string& path) {
    error_code filesystemError;
    const bool fileExists = filesystem::exists(path, filesystemError);
    if (filesystemError) {
        throw runtime_error("Could not check review interval file: " +
                            filesystemError.message());
    }
    if (!fileExists) {
        return false;
    }

    ifstream file(path);
    if (!file) {
        throw runtime_error("Could not open review interval file: " + path);
    }

    string line;
    if (!getline(file, line)) {
        throw runtime_error("Review interval file is empty: " + path);
    }
    string extraLine;
    if (getline(file, extraLine)) {
        throw runtime_error("Review interval file must contain one line: " +
                            path);
    }
    if (file.bad()) {
        throw runtime_error("Could not read review interval file: " + path);
    }

    array<int, 5> loadedIntervals{};
    istringstream values(line);
    string token;
    for (size_t index = 0; index < loadedIntervals.size(); ++index) {
        if (!getline(values, token, ',')) {
            throw runtime_error("Invalid review interval file: " + path);
        }
        istringstream number(token);
        if (!(number >> loadedIntervals[index]) ||
            number.peek() != char_traits<char>::eof()) {
            throw runtime_error("Invalid review interval file: " + path);
        }
    }
    if (getline(values, token, ',')) {
        throw runtime_error("Invalid review interval file: " + path);
    }

    try {
        setIntervals(loadedIntervals);
    } catch (const invalid_argument&) {
        throw runtime_error("Review intervals must be positive and strictly "
                            "increasing: " + path);
    }
    return true;
}

void ReviewScheduler::saveIntervals(const string& path) const {
    const filesystem::path parent = filesystem::path(path).parent_path();
    if (!parent.empty()) {
        error_code filesystemError;
        filesystem::create_directories(parent, filesystemError);
        if (filesystemError) {
            throw runtime_error("Could not create review settings directory: " +
                                filesystemError.message());
        }
    }

    ofstream file(path);
    if (!file) {
        throw runtime_error("Could not open review interval file for writing: " +
                            path);
    }

    for (size_t index = 0; index < intervalDays.size(); ++index) {
        if (index != 0) {
            file << ',';
        }
        file << intervalDays[index];
    }
    file << '\n';
    if (!file) {
        throw runtime_error("Could not save review intervals: " + path);
    }
}

void ReviewScheduler::markResult(Word& word, bool correct, time_t now) {
    if (!correct) {
        word.setReviewState(0, addDays(now, intervalDays[0]));
        return;
    }

    const int currentCount = max(0, word.getReviewCount());
    const int nextCount =
        min(currentCount + 1, static_cast<int>(intervalDays.size()));

    word.setReviewState(nextCount,
                        addDays(now, intervalDays[nextCount - 1]));
}
