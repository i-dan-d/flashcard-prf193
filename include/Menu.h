#pragma once

#include <string>

#include "CsvManager.h"
#include "Deck.h"
#include "FileManager.h"
#include "ReviewScheduler.h"
#include "WordLister.h"
#include "SearchService.h"

#include <iostream>
using namespace std;

class Menu {
private:
    Deck deck;
    WordLister lister;
    ReviewScheduler scheduler;
    CsvManager csv;
    FileManager fileManager;
    SearchService search;
    const string dataPath = "data/words.bin";

    void handleAddEditDelete();
    void handleSearch();
    void handleList();
    void handleReview();
    void handleImportExport();

    int readChoice() const;
    void loadOnStart();
    void saveOnExit();
public:
    Menu();
    void run();
};
