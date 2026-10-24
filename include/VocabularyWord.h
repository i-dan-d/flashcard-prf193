#include "Word.h"

class VocabularyWord: Word{
private:
public:
    VocabularyWord(string id, string term, string meaning,
    string partOfSpeech, string type);
    string getType(); //override -> Tra ve Vocabulary
};