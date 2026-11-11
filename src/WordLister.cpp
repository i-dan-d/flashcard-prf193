
#include <algorithm>
#include "../include/WordLister.h"
#include "../include/Word.h"
#include "../include/Deck.h"

using namespace std;

WordLister::WordLister(const Deck& deck) //nguồn dữ liệu, không thể thay đổi.
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


    const auto& words = deck.all(); //lấy danh sách từ deck
    for(const auto& word : words){ //đưa địa chỉ của từng word vào result
    result.push_back(word.get());
    }
    if (key == SortKey::Alphabet){ // kiểm tra tiêu chí và sắp xếp
        sort(result.begin(), result.end(),lessByAlphabet);
    }
    else if(key == SortKey::ReviewCount){
        sort(result.begin(), result.end(), lessByReviewCount);
    }

    return result; // trả kết quả
}

string WordLister::format(const Word& word){
    return  word.getTerm() + " (" +
            word.getPartOfSpeech() + ") - "+
            word.getMeaning();
} // apple (noun) - quả táo 