// Word.h — khai báo
#include <string>>
class Word {
public:
    Word(std::string term, std::string meaning);
    std::string getTerm() const;
    virtual std::string getExampleSentence() const;  // chỉ khai báo, không có nội dung
private:
    std::string term, meaning;
};