#pragma once

#include "Word.h"

class PhraseWord : public Word {
public:
    PhraseWord(int id, const string& term,
               const string& partOfSpeech,
               const string& meaning);

    string getTypeName() const override;
};
