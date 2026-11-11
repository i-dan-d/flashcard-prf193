#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <iostream>
using namespace std;

class Deck;
class Word;

class FileManager {
public:
    void save(const Deck& deck, const string& path) const;
    void load(Deck& deck, const string& path) const;
    bool exists(const string& path) const;
};
