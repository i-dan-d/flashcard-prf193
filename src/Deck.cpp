#include "../include/Deck.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

#include "../include/StringUtil.h"
#include "../include/Word.h"

Deck::Deck() = default;

Deck::~Deck() = default;

Deck::Deck(Deck&&) noexcept = default;

Deck& Deck::operator=(Deck&&) noexcept = default;

bool Deck::addWord(const string& term,
                   const string& partOfSpeech,
                   const string& meaning) {
    if (findByTerm(term) != nullptr) {
        return false;
    }

    words.push_back(Word::create(nextId, term, partOfSpeech, meaning));
    ++nextId;
    return true;
}

bool Deck::editWord(const int& id,
                    const string& newTerm,
                    const string& newPartOfSpeech,
                    const string& newMeaning) {
    const auto it = find_if(words.begin(), words.end(),
                            [id](const unique_ptr<Word>& word) {
                                return word->getId() == id;
                            });
    if (it == words.end()) {
        return false;
    }

    const Word* duplicate = findByTerm(newTerm);
    if (duplicate != nullptr && duplicate->getId() != id) {
        return false;
    }

    const int reviewCount = (*it)->getReviewCount();
    const time_t nextReviewDate = (*it)->getNextReviewDate();
    unique_ptr<Word> updatedWord =
        Word::create(id, newTerm, newPartOfSpeech, newMeaning);
    updatedWord->setReviewState(reviewCount, nextReviewDate);
    *it = move(updatedWord);
    return true;
}

bool Deck::removeById(const int& id) {
    const auto it = find_if(words.begin(), words.end(),
                            [id](const unique_ptr<Word>& word) {
                                return word->getId() == id;
                            });
    if (it == words.end()) {
        return false;
    }

    words.erase(it);
    return true;
}

Word* Deck::findById(const int& id) {
    for (const auto& word : words) {
        if (word->getId() == id) {
            return word.get();
        }
    }
    return nullptr;
}

const Word* Deck::findById(const int& id) const {
    for (const auto& word : words) {
        if (word->getId() == id) {
            return word.get();
        }
    }
    return nullptr;
}

Word* Deck::findByTerm(const string& term) {
    for (const auto& word : words) {
        if (StringUtil::equalsIgnoreCase(word->getTerm(), term)) {
            return word.get();
        }
    }
    return nullptr;
}

const Word* Deck::findByTerm(const string& term) const {
    for (const auto& word : words) {
        if (StringUtil::equalsIgnoreCase(word->getTerm(), term)) {
            return word.get();
        }
    }
    return nullptr;
}

void Deck::adopt(unique_ptr<Word> word) {
    if (word == nullptr) {
        throw invalid_argument("Cannot add a null Word to Deck.");
    }

    nextId = max(nextId, word->getId() + 1);
    words.push_back(move(word));
}

const vector<unique_ptr<Word>>& Deck::all() const {
    return words;
}

size_t Deck::size() const {
    return words.size();
}

bool Deck::empty() const {
    return words.empty();
}

void Deck::clear() {
    words.clear();
}
