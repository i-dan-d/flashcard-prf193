#include <iostream>
#include "../include/Word.h"
 
int main() {
    auto term1 = Word::create(1, "handsome", "adj", "đep trai/ hào phóng");
    cout << term1->getTypeName() <<endl;

    auto term2 = Word::create(3, "Duy handsome", "adj", "Duy đep trai/ hào phóng");
    cout << term2->getTypeName() <<endl;
    
 
    return 0;
}