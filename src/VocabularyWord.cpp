#include "../include/VocabularyWord.h"

VocabularyWord::VocabularyWord(int id, const string& term,
                       const string& partOfSpeech,
                       const string& meaning)
    : Word(id, term, partOfSpeech, meaning) {}

string VocabularyWord::getTypeName() const {
    return "Vocabulary";
}
