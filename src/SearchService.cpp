#include "SearchService.h"
#include "Deck.h"
#include "Word.h"
#include <algorithm>
#include <cctype>

static string toLowerStr(string s) {
    transform(s.begin(), s.end(), s.begin(),
              [](unsigned char c) { return tolower(c); });
    return s;
}

SearchService::SearchService(const Deck& deck) : deck(deck) {}

bool SearchService::matches(const Word& word, const string& keyLower) const {
    string term = toLowerStr(word.getTerm());
    string meaning = toLowerStr(word.getMeaning());
    return term.find(keyLower) != string::npos ||
           meaning.find(keyLower) != string::npos;
}

vector<const Word*> SearchService::search(const string& keyword) const {
    vector<const Word*> result;
    string key = toLowerStr(keyword);
    if (key.empty()) return result;

    // DONG DUY NHAT CAN KIEM TRA: ten ham lay danh sach tu trong Deck.h
    for (const auto& w : deck.all()) {
        const Word* p = &*w;   // dung duoc cho ca unique_ptr lan con tro thuong
        if (matches(*p, key)) result.push_back(p);
    }
    return result;

}
