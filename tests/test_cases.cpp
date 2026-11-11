#include "CsvManager.h"
#include "Deck.h"
#include "FileManager.h"
#include "PhraseWord.h"
#include "ReviewScheduler.h"
#include "SearchService.h"
#include "StringUtil.h"
#include "VocabularyWord.h"
#include "Word.h"
#include "WordLister.h"

#include <chrono>
#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

class TestFailure : public std::runtime_error {
public:
    explicit TestFailure(const std::string& message)
        : std::runtime_error(message) {}
};

void require(bool condition, const std::string& message) {
    if (!condition) {
        throw TestFailure(message);
    }
}

template <typename Exception, typename Function>
void requireThrows(Function&& function, const std::string& message) {
    try {
        function();
    } catch (const Exception&) {
        return;
    }
    throw TestFailure(message);
}

class TemporaryDirectory {
private:
    std::filesystem::path path;

public:
    TemporaryDirectory() {
        const auto timestamp =
            std::chrono::steady_clock::now().time_since_epoch().count();
        path = std::filesystem::temp_directory_path() /
               ("flashcard_tests_" + std::to_string(timestamp));
        std::filesystem::create_directories(path);
    }

    ~TemporaryDirectory() {
        std::error_code error;
        std::filesystem::remove_all(path, error);
    }

    std::filesystem::path file(const std::string& name) const {
        return path / name;
    }
};

void writeTextFile(const std::filesystem::path& path,
                   const std::string& contents) {
    std::ofstream file(path);
    if (!file) {
        throw TestFailure("Could not create test fixture: " + path.string());
    }
    file << contents;
}

std::string readTextFile(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file) {
        throw TestFailure("Could not read test output: " + path.string());
    }
    return std::string(std::istreambuf_iterator<char>(file),
                       std::istreambuf_iterator<char>());
}

void testWordFactoryAndStringUtilities() {
    std::unique_ptr<Word> vocabulary =
        Word::create(7, "apple", "noun", "fruit");
    std::unique_ptr<Word> phrase =
        Word::create(8, "look up", "verb", "search");

    require(vocabulary->getTypeName() == "Vocabulary",
            "single-word term should create VocabularyWord");
    require(phrase->getTypeName() == "Phrase",
            "term containing whitespace should create PhraseWord");
    require(vocabulary->getId() == 7 && vocabulary->getReviewCount() == 0 &&
                vocabulary->getNextReviewDate() == 0,
            "Word constructor should initialize identity and review state");

    require(StringUtil::toLower("HeLLo") == "hello",
            "toLower should normalize case");
    require(StringUtil::trim("  word \t") == "word",
            "trim should remove surrounding whitespace");
    require(StringUtil::isBlank(" \t\r\n"),
            "isBlank should recognize whitespace-only input");
    require(StringUtil::equalsIgnoreCase("Apple", "aPPLe"),
            "equalsIgnoreCase should ignore case");
    require(StringUtil::containsIgnoreCase("Green Apple", "APPLE"),
            "containsIgnoreCase should find case-insensitive substrings");
}

void testDeckCrudAndDuplicateHandling() {
    Deck deck;
    require(deck.empty() && deck.size() == 0,
            "new Deck should be empty");
    require(deck.addWord("apple", "noun", "fruit"),
            "Deck should accept a new word");
    require(!deck.addWord("APPLE", "noun", "duplicate"),
            "Deck should reject case-insensitive duplicate terms");

    Word* apple = deck.findByTerm("APPLE");
    require(apple != nullptr && apple->getId() == 1,
            "findByTerm should be case-insensitive and IDs should start at 1");
    const int appleId = apple->getId();
    apple->setReviewState(2, 1234);

    require(deck.editWord(appleId, "green apple", "noun",
                          "a fruit"),
            "editWord should update an existing word");
    const Word* edited = deck.findById(appleId);
    require(edited != nullptr && edited->getTerm() == "green apple" &&
                edited->getReviewCount() == 2 &&
                edited->getNextReviewDate() == 1234 &&
                edited->getTypeName() == "Phrase",
            "editWord should preserve review state and recalculate word type");
    require(deck.addWord("pear", "noun", "fruit"),
            "editing should not cause the next ID to be reused");
    require(deck.findByTerm("missing") == nullptr &&
                deck.findById(999) == nullptr,
            "find operations should return nullptr for missing words");

    const int pearId = deck.findByTerm("pear")->getId();
    require(!deck.editWord(pearId, "GREEN APPLE", "noun", "duplicate"),
            "editWord should reject a term used by another word");
    require(!deck.editWord(999, "new", "noun", "meaning"),
            "editWord should reject an unknown ID");
    require(deck.removeById(pearId) && !deck.removeById(pearId),
            "removeById should remove once and reject an unknown ID");
    deck.clear();
    require(deck.empty(), "clear should remove all words");
}

void testDeckAdoptAndNextId() {
    Deck deck;
    deck.adopt(std::make_unique<VocabularyWord>(12, "adopted", "noun",
                                                 "taken in"));
    require(deck.findById(12) != nullptr,
            "adopt should add a preconstructed Word");
    require(deck.addWord("next", "noun", "following"),
            "Deck should accept a word after adopt");
    require(deck.findByTerm("next")->getId() == 13,
            "adopt should advance the next ID past the adopted Word");
    requireThrows<std::invalid_argument>(
        [&deck] { deck.adopt(nullptr); },
        "adopt should reject a null Word");
}

void testSearchByTermAndMeaning() {
    Deck deck;
    deck.addWord("Apple", "noun", "A red fruit");
    deck.addWord("look up", "verb", "search for information");
    deck.addWord("pear", "noun", "green fruit");

    const SearchService searcher(deck);
    const auto termMatches = searcher.search("LOOK");
    require(termMatches.size() == 1 &&
                termMatches.front()->getTerm() == "look up",
            "search should find case-insensitive term substrings");
    const auto meaningMatches = searcher.search("FRUIT");
    require(meaningMatches.size() == 2,
            "search should find case-insensitive meaning substrings");
    require(searcher.search("").empty() && searcher.search("absent").empty(),
            "empty and unmatched searches should return no results");
}

void testWordListingAndFormatting() {
    Deck deck;
    deck.addWord("banana", "noun", "yellow fruit");
    deck.addWord("apple", "noun", "red fruit");
    deck.addWord("cat", "noun", "animal");
    deck.findByTerm("banana")->setReviewState(5, 0);
    deck.findByTerm("apple")->setReviewState(2, 0);
    deck.findByTerm("cat")->setReviewState(8, 0);

    const WordLister lister(deck);
    const auto alphabet = lister.sortedBy(WordLister::SortKey::Alphabet);
    require(alphabet.size() == 3 && alphabet[0]->getTerm() == "apple" &&
                alphabet[1]->getTerm() == "banana" &&
                alphabet[2]->getTerm() == "cat",
            "alphabet sort should return terms in ascending order");
    const auto reviewCount =
        lister.sortedBy(WordLister::SortKey::ReviewCount);
    require(reviewCount[0]->getReviewCount() == 2 &&
                reviewCount[1]->getReviewCount() == 5 &&
                reviewCount[2]->getReviewCount() == 8,
            "review-count sort should return counts in ascending order");
    require(WordLister::format(*alphabet[0]) ==
                "apple (noun) - red fruit",
            "format should produce the documented display string");

    Deck emptyDeck;
    require(WordLister(emptyDeck)
                .sortedBy(WordLister::SortKey::Alphabet)
                .empty(),
            "sorting an empty Deck should return an empty vector");
}

void testReviewSchedulingAndAnswers() {
    constexpr std::time_t now = 1700000000;
    constexpr std::time_t day = 24 * 60 * 60;
    Deck deck;
    deck.addWord("later", "noun", "later meaning");
    deck.addWord("sooner", "noun", "sooner meaning");
    deck.addWord("overdue", "noun", "overdue meaning");
    deck.findByTerm("later")->setReviewState(0, now + 20);
    deck.findByTerm("sooner")->setReviewState(0, now);
    deck.findByTerm("overdue")->setReviewState(0, now - day);

    ReviewScheduler scheduler(deck);
    const auto due = scheduler.dueWords(now);
    require(due.size() == 2 && due.front()->getTerm() == "overdue" &&
                due.back()->getTerm() == "sooner",
            "dueWords should include dates at now and exclude future dates");
    require(scheduler.nextDue(now) == due.front(),
            "nextDue should return the earliest due Word");
    require(scheduler.nextDue(now + 21) != nullptr,
            "nextDue should return a Word when due items exist");
    Word* sooner = deck.findByTerm("sooner");
    require(scheduler.checkAnswer(*sooner, "SOONER MEANING"),
            "checkAnswer should ignore case");
    require(!scheduler.checkAnswer(*sooner, "wrong"),
            "checkAnswer should reject an incorrect answer");

    scheduler.markResult(*sooner, true, now);
    require(sooner->getReviewCount() == 1 &&
                sooner->getNextReviewDate() == now + day,
            "first correct answer should schedule one day later");
    scheduler.markResult(*sooner, true, now);
    require(sooner->getReviewCount() == 2 &&
                sooner->getNextReviewDate() == now + 3 * day,
            "second correct answer should schedule three days later");
    scheduler.markResult(*sooner, true, now);
    scheduler.markResult(*sooner, true, now);
    scheduler.markResult(*sooner, true, now);
    scheduler.markResult(*sooner, true, now);
    require(sooner->getReviewCount() == 5 &&
                sooner->getNextReviewDate() == now + 30 * day,
            "review interval should cap at 30 days and review count at five");
    scheduler.markResult(*sooner, false, now);
    require(sooner->getReviewCount() == 0 &&
                sooner->getNextReviewDate() == now + day,
            "an incorrect answer should reset count and schedule one day later");

    Deck emptyDeck;
    require(ReviewScheduler(emptyDeck).nextDue(now) == nullptr,
            "nextDue should return nullptr when no words are due");
}

void testReviewIntervalsAndMultipleChoice() {
    constexpr std::time_t now = 1700000000;
    constexpr std::time_t day = 24 * 60 * 60;
    TemporaryDirectory temporary;
    const auto settingsPath = temporary.file("review_intervals.csv");

    Deck deck;
    deck.addWord("apple", "noun", "fruit");
    deck.addWord("book", "noun", "pages");
    deck.addWord("chair", "noun", "furniture");
    deck.addWord("desk", "noun", "table");
    deck.addWord("pear", "noun", "fruit");

    ReviewScheduler scheduler(deck);
    scheduler.setIntervals({2, 4, 8, 16, 32});
    Word* apple = deck.findByTerm("apple");
    scheduler.markResult(*apple, true, now);
    require(apple->getReviewCount() == 1 &&
                apple->getNextReviewDate() == now + 2 * day,
            "custom first interval should control the next review date");

    std::mt19937 random(7);
    const auto choices = scheduler.multipleChoiceChoices(*apple, random);
    require(choices.size() == 4 &&
                std::count(choices.begin(), choices.end(), apple) == 1,
            "multiple-choice should include the answer and three distractors");
    for (size_t index = 0; index < choices.size(); ++index) {
        require(choices[index] != nullptr,
                "multiple-choice options should not be null");
        for (size_t other = index + 1; other < choices.size(); ++other) {
            require(!StringUtil::equalsIgnoreCase(
                        choices[index]->getMeaning(),
                        choices[other]->getMeaning()),
                    "multiple-choice meanings should be distinct");
        }
    }

    scheduler.saveIntervals(settingsPath.string());
    ReviewScheduler restoredScheduler(deck);
    require(restoredScheduler.loadIntervals(settingsPath.string()) &&
                restoredScheduler.intervals() ==
                    std::array<int, 5>{2, 4, 8, 16, 32},
            "custom review intervals should persist and load");
    require(!restoredScheduler.loadIntervals(
                temporary.file("missing_intervals.csv").string()),
            "missing settings should keep default review intervals");

    requireThrows<std::invalid_argument>(
        [&] { scheduler.setIntervals({1, 3, 2, 15, 30}); },
        "review intervals should reject non-increasing schedules");

    Deck smallDeck;
    smallDeck.addWord("only", "noun", "single answer");
    ReviewScheduler smallScheduler(smallDeck);
    const auto singleChoice = smallScheduler.multipleChoiceChoices(
        *smallDeck.findByTerm("only"), random);
    require(singleChoice.size() == 1 &&
                singleChoice.front() == smallDeck.findByTerm("only"),
            "multiple-choice should adapt when the deck has no distractors");
}

void testCsvImportValidationAndQuoting() {
    TemporaryDirectory temporary;
    const auto inputPath = temporary.file("import.csv");
    writeTextFile(inputPath,
                  "apple,noun,fruit\n"
                  "\"look up\",verb,search\n"
                  "\"say \"\"hello\"\"\",verb,speak\n"
                  "remember,verb,recall,2,1700001000\n"
                  "bad,noun,entry,not-a-level,1700001000\n"
                  "bad,line\n"
                  "APPLE,noun,duplicate\n");

    Deck deck;
    CsvManager csv;
    int skipped = -1;
    const int imported =
        csv.importFrom(inputPath.string(), deck, skipped);
    require(imported == 4 && skipped == 3 && deck.size() == 4,
            "CSV import should count valid, malformed, and duplicate rows");
    require(deck.findByTerm("say \"hello\"") != nullptr,
            "CSV import should decode escaped quotation marks");
    require(deck.findByTerm("look up")->getTypeName() == "Phrase",
            "imported terms should use the Word factory's type rule");
    require(deck.findByTerm("remember")->getReviewCount() == 2 &&
                deck.findByTerm("remember")->getNextReviewDate() == 1700001000 &&
                deck.findByTerm("remember")->getId() == 4,
            "five-field CSV should restore review state and assign IDs locally");
}

void testCsvExportImportRoundTrip() {
    TemporaryDirectory temporary;
    const auto exportPath = temporary.file("export.csv");
    Deck original;
    original.addWord("look up", "verb", "search, find");
    original.addWord("say \"hello\"", "verb", "greet");
    original.findByTerm("look up")->setReviewState(2, 1700001000);
    original.findByTerm("say \"hello\"")->setReviewState(4, 1700002000);

    CsvManager csv;
    csv.exportTo(exportPath.string(), original);
    const std::string exported = readTextFile(exportPath);
    require(exported ==
                "look up,verb,\"search, find\",2,1700001000\n"
                "\"say \"\"hello\"\"\",verb,greet,4,1700002000\n",
            "CSV export should write five fields, omit IDs, and escape commas "
            "and quotes");

    Deck imported;
    int skipped = -1;
    require(csv.importFrom(exportPath.string(), imported, skipped) == 2 &&
                skipped == 0 && imported.size() == original.size(),
            "exported CSV should import without skipped rows");
    require(imported.findByTerm("look up")->getMeaning() == "search, find" &&
                imported.findByTerm("say \"hello\"")->getMeaning() == "greet" &&
                imported.findByTerm("look up")->getReviewCount() == 2 &&
                imported.findByTerm("look up")->getNextReviewDate() ==
                    1700001000 &&
                imported.findByTerm("say \"hello\"")->getReviewCount() == 4 &&
                imported.findByTerm("say \"hello\"")->getNextReviewDate() ==
                    1700002000 &&
                imported.findByTerm("look up")->getId() == 1 &&
                imported.findByTerm("say \"hello\"")->getId() == 2,
            "CSV round trip should preserve word and review data with new IDs");
}

void testCsvMissingFileError() {
    TemporaryDirectory temporary;
    Deck deck;
    int skipped = 0;
    CsvManager csv;
    requireThrows<std::runtime_error>(
        [&] {
            csv.importFrom(temporary.file("missing.csv").string(), deck,
                           skipped);
        },
        "CSV import should report a missing input file");
}

void testFileManagerRoundTripAndErrors() {
    TemporaryDirectory temporary;
    const auto csvPath = temporary.file("words.csv");
    const auto corruptPath = temporary.file("corrupt.csv");
    Deck original;
    original.addWord("apple", "noun", "fruit");
    original.addWord("look up", "verb", "search");
    original.findByTerm("apple")->setReviewState(3, 1700001000);
    original.findByTerm("look up")->setMeaning("search, \"find\"\nlook for");

    FileManager files;
    files.save(original, csvPath.string());
    require(files.exists(csvPath.string()),
            "save should create a file recognized by exists");

    Deck loaded;
    loaded.addWord("discarded", "noun", "old data");
    files.load(loaded, csvPath.string());
    require(loaded.size() == 2 &&
                loaded.findByTerm("apple")->getTypeName() == "Vocabulary" &&
                loaded.findByTerm("look up")->getTypeName() == "Phrase" &&
                loaded.findByTerm("apple")->getReviewCount() == 3 &&
                loaded.findByTerm("apple")->getNextReviewDate() == 1700001000 &&
                loaded.findByTerm("look up")->getMeaning() ==
                    "search, \"find\"\nlook for",
            "CSV load should restore words, types, review state, and quoting");
    require(loaded.addWord("next", "noun", "following") &&
                loaded.findByTerm("next")->getId() == 3,
            "loaded Deck should continue IDs after the largest saved ID");

    writeTextFile(corruptPath, "not a flashcard file");
    const std::size_t oldSize = loaded.size();
    requireThrows<std::runtime_error>(
        [&] { files.load(loaded, corruptPath.string()); },
        "load should reject an invalid CSV file");
    require(loaded.size() == oldSize,
            "failed load should leave the existing Deck unchanged");
    requireThrows<std::runtime_error>(
        [&] { files.load(loaded, temporary.file("missing.csv").string()); },
        "load should report a missing CSV file");

    const auto legacyPath = temporary.file("legacy.csv");
    writeTextFile(legacyPath, "apple,noun,fruit\nlook up,verb,search\n");
    files.load(loaded, legacyPath.string());
    require(loaded.size() == 2 &&
                loaded.findByTerm("apple")->getId() == 1 &&
                loaded.findByTerm("look up")->getTypeName() == "Phrase",
            "load should accept the existing three-column CSV format");
}

}  // namespace

int main() {
    const std::vector<std::pair<std::string, std::function<void()>>> tests = {
        {"Word factory and string utilities", testWordFactoryAndStringUtilities},
        {"Deck CRUD and duplicate handling", testDeckCrudAndDuplicateHandling},
        {"Deck adopt and next ID", testDeckAdoptAndNextId},
        {"Search by term and meaning", testSearchByTermAndMeaning},
        {"Word listing and formatting", testWordListingAndFormatting},
        {"Review scheduling and answers", testReviewSchedulingAndAnswers},
        {"Review intervals and multiple choice",
         testReviewIntervalsAndMultipleChoice},
        {"CSV import validation and quoting", testCsvImportValidationAndQuoting},
        {"CSV export/import round trip", testCsvExportImportRoundTrip},
        {"CSV missing-file error", testCsvMissingFileError},
        {"File manager round trip and errors", testFileManagerRoundTripAndErrors},
    };

    int passed = 0;
    int failed = 0;
    for (const auto& test : tests) {
        try {
            test.second();
            ++passed;
            std::cout << "[PASS] " << test.first << '\n';
        } catch (const std::exception& error) {
            ++failed;
            std::cout << "[FAIL] " << test.first << ": "
                      << error.what() << '\n';
        }
    }

    std::cout << "Tests: " << passed + failed << ", Passed: " << passed
              << ", Failed: " << failed << '\n';
    return failed == 0 ? 0 : 1;
}
