#pragma once

#include "Word.h"

class PhraseWord : public Word {
public:
    PhraseWord(int id, const std::string& term,
               const std::string& partOfSpeech,
               const std::string& meaning);

    std::string getTypeName() const override;
};
