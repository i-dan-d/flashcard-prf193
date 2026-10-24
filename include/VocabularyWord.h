#pragma once

#include "Word.h"

class VocabularyWord : public Word {
public:
    VocabularyWord(int id, const std::string& term,
                   const std::string& partOfSpeech,
                   const std::string& meaning);

    std::string getTypeName() const override;
};
