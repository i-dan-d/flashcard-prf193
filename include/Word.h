#pragma once

#include <ctime>
#include <memory>
#include <string>

class Word {
public:
    Word(int id, const std::string& term, const std::string& partOfSpeech,
         const std::string& meaning);
    virtual ~Word() = default;

    int getId() const;
    const std::string& getTerm() const;
    const std::string& getPartOfSpeech() const;
    const std::string& getMeaning() const;
    int getReviewCount() const;
    std::time_t getNextReviewDate() const;

    void setTerm(const std::string& term);
    void setPartOfSpeech(const std::string& partOfSpeech);
    void setMeaning(const std::string& meaning);
    void setReviewState(int count, std::time_t nextReviewDate);

    virtual std::string getTypeName() const = 0;

    static std::unique_ptr<Word> create(
        int id,
        const std::string& term,
        const std::string& partOfSpeech,
        const std::string& meaning);

private:
    int idWord;
    std::string term;
    std::string partOfSpeech;
    std::string meaning;
    int reviewCount;
    std::time_t nextReviewDate;
};
