// Word.h — khai báo
#include <iostream>
using namespace std;
class Word {
// chỉ khai báo, không có nội dung
private:
    string idWord, term, meaning, partOfSpeech, type;
    int reviewCount;
    time_t nextReviewDate;
public:
    Word(string id, string term, string mean, char partSpeech[3],
    string type, int reviewC, time_t dataNext);
    virtual ~Word();
    string getId() const;
    string getTerm() const;
    string getMeaning() const;
    string getPartOfSpeech() const;
    string getType() const;
    int getReviewCount() const;
    time_t getNextReviewDate() const;

    string setTerm() ;
    string setMeaning() ;
    string setPartOfSpeech() ;
    int setReviewState(int count, time_t next);

    static unique_ptr<Word> creat(string id, string term, string meaning, 
    string partOfSpeech, string type);
};