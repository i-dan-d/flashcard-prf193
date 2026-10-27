#include <iostream>
#include <string>
#include <vector>
#include "SearchService.h"
#include "Deck.h"
#include "Word.h"

void menuSearch(const Deck& deck) {
    std::string keyword;
    std::cout << "Nhap tu khoa: ";
    std::getline(std::cin, keyword);

    SearchService service(deck);
    std::vector<const Word*> result = service.search(keyword);

    if (result.empty()) {
        std::cout << "Khong tim thay ket qua.\n";
        return;
    }
    std::cout << "Tim thay " << result.size() << " ket qua:\n";
    for (const Word* w : result) {
        std::cout << "- " << w->getTerm() << ": " << w->getMeaning() << "\n";
    }
}