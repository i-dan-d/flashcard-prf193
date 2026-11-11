#include "Deck.h"
#include "SearchService.h"
#include "Word.h"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace {

string formatReviewDate(time_t reviewDate) {
    if (reviewDate <= 0) {
        return "Due now (no review scheduled)";
    }

    tm localTime = {};
#ifdef _WIN32
    if (localtime_s(&localTime, &reviewDate) != 0) {
        return to_string(static_cast<long long>(reviewDate));
    }
#else
    if (localtime_r(&reviewDate, &localTime) == nullptr) {
        return to_string(static_cast<long long>(reviewDate));
    }
#endif

    ostringstream formatted;
    formatted << put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    return formatted.str();
}

}  // namespace

vector<const Word*> SearchService::menuSearch() const {
    std::string keyword;
    std::cout << "Nhap tu khoa: ";
    if (!std::getline(std::cin, keyword)) {
        return {};
    }

    const vector<const Word*> result = search(keyword);

    if (result.empty()) {
        std::cout << "Khong tim thay ket qua.\n";
        return result;
    }
    std::cout << "Tim thay " << result.size() << " ket qua:\n";
    for (const Word* word : result) {
        std::cout << "\nID: " << word->getId()
                //   << "Type: " << word->getTypeName()
                  << " - " << word->getTerm()
                  << " - " << word->getPartOfSpeech()
                  << " - " << word->getMeaning()
                  << " - " << word->getReviewCount()
                  << " - "
                  << formatReviewDate(word->getNextReviewDate()) << '\n';
    }
    return result;
}
