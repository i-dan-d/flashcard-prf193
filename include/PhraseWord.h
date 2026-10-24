#include "Word.h"

class PhraseWord:Word{
private:
public:
    PhraseWord(string id, string term, string meaning,
    string partOfSpeech, string type);
    string getType(); //override -> tra ve Phrase
};