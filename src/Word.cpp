#include "../include/Word.h"

#include <algorithm>
#include <cctype>

#include "../include/PhraseWord.h"
#include "../include/VocabularyWord.h"

Word::Word(int id, const string& term, const string& partOfSpeech,
           const string& meaning)
    : idWord(id),
      term(term),
      partOfSpeech(partOfSpeech),
      meaning(meaning),
      reviewCount(0),
      nextReviewDate(0) {}

int Word::getId() const {
    return idWord;
}

const string& Word::getTerm() const {
    return term;
}

const string& Word::getPartOfSpeech() const {
    return partOfSpeech;
}

const string& Word::getMeaning() const {
    return meaning;
}

int Word::getReviewCount() const {
    return reviewCount;
}

time_t Word::getNextReviewDate() const {
    return nextReviewDate;
}

void Word::setTerm(const string& newTerm) {
    term = newTerm;
}

void Word::setPartOfSpeech(const string& newPartOfSpeech) {
    partOfSpeech = newPartOfSpeech;
}

void Word::setMeaning(const string& newMeaning) {
    meaning = newMeaning;
}

void Word::setReviewState(int count, time_t next) {
    reviewCount = count;
    nextReviewDate = next;
}

unique_ptr<Word> Word::create(
    int id,
    const string& term,
    const string& partOfSpeech,
    const string& meaning) {
    const bool isPhrase = any_of(   // chỗ này chỉ mới kiểm tra xem có trong chuỗi có khoảng trắng không, tránh đầu vào là rỗng thì xử lý ở ngoài
        term.begin(), term.end(),
        [](unsigned char character) { return isspace(character) != 0; });

    if (isPhrase) {
        return make_unique<PhraseWord>(id, term, partOfSpeech, meaning);
    }

    return make_unique<VocabularyWord>(id, term, partOfSpeech, meaning);
}
