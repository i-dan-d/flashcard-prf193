// Word.cpp — định nghĩa (implementation thật)
#include "../include/Word.h"
#include <iostream>
Word::Word(std::string t, std::string m) : term(t), meaning(m) {}
std::string Word::getTerm() const { return term; }
std::string Word::getExampleSentence() const {
    return "Example for " + term + meaning;  // logic thật nằm ở đây
}