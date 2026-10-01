#include <iostream>
#include "../include/Word.h"
 
int main() {
    Word w("Serendipity", "may man tinh co, tinh co phat hien dieu hay");
 
    std::cout << "Term: " << w.getTerm() << std::endl;
    std::cout << w.getExampleSentence() << std::endl;
 
    return 0;
}