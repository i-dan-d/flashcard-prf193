#pragma once

#include "Word.h"

class VocabularyWord : public Word {
public:
    VocabularyWord(int id, const string& term,
                   const string& partOfSpeech,
                   const string& meaning);

    string getTypeName() const override;
};
