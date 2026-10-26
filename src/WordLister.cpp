
#include <algorithm>
#include "../include/WordLister.h"
#include "../include/Word.h"
#include "../include/Deck.h"

using namespace std;

WordLister::WordLister(const Deck& deck)
    : deck(deck){

}
bool WordLister::lessByAlphabet(
    const Word* left,
    const Word* right
){
    return left->getTerm() < right->getTerm();
}

bool WordLister::lessByReviewCount(
    const Word* left,
    const Word* right
){
    return left->getReviewCount() < right->getReviewCount();
}

vector<const Word*> WordLister::sortedBy(SortKey key) const{
    vector<const Word*> result; //biến result là danh sách kết quả mà hàm sẽ trả về 


    const auto& words = deck.all();
    for(const auto& word : words){
    result.push_back(word.get());
    }
    if (key == SortKey::Alphabet){
        sort(result.begin(), result.end(),lessByAlphabet);
    }
    else if(key == SortKey::ReviewCount){
        sort(result.begin(), result.end(), lessByReviewCount);
    }

    return result;
}

string WordLister::format(const Word& word){
    return  word.getTerm() + " (" +
            word.getPartOfSpeech() + ") - "+
            word.getMeaning();
}