#include "../include/PhraseWord.h"

PhraseWord::PhraseWord(int id, const string& term,
                       const string& partOfSpeech,
                       const string& meaning)
    : Word(id, term, partOfSpeech, meaning) {}

string PhraseWord::getTypeName() const {
    return "Phrase";
}
