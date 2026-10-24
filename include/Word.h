#pragma once

#include <ctime>
#include <memory>
#include <string>
#include <iostream>

using namespace std;

class Word {
private:
    int idWord;
    string term;
    string partOfSpeech;
    string meaning;
    int reviewCount;
    time_t nextReviewDate;
public:
    Word(int id, const string& term, const string& partOfSpeech,
         const string& meaning);
    virtual ~Word() = default;

    int getId() const;
    const string& getTerm() const;
    const string& getPartOfSpeech() const;
    const string& getMeaning() const;
    int getReviewCount() const;
    time_t getNextReviewDate() const;

    void setTerm(const string& term);
    void setPartOfSpeech(const string& partOfSpeech);
    void setMeaning(const string& meaning);
    void setReviewState(int count, time_t nextReviewDate);

    virtual string getTypeName() const = 0;

    static unique_ptr<Word> create(
        int id,
        const string& term,
        const string& partOfSpeech,
        const string& meaning);

};
