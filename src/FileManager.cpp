#include "../include/FileManager.h"

#include "../include/Deck.h"
#include "../include/PhraseWord.h"
#include "../include/VocabularyWord.h"
#include "../include/Word.h"

#include <stdexcept>
#include <utility>

void FileManager::writeString(ofstream& stream, const string& value) const {
    uint32_t length = static_cast<uint32_t>(value.size());
    stream.write(reinterpret_cast<const char*>(&length), sizeof(length));
    stream.write(value.c_str(), length);

    if (!stream) {
        throw runtime_error("Could not write string to file.");
    }
}

string FileManager::readString(ifstream& stream) const {
    uint32_t length;
    stream.read(reinterpret_cast<char*>(&length), sizeof(length));
    if (!stream) {
        throw runtime_error("Could not read string length from file.");
    }

    string value(length, '\0');
    stream.read(&value[0], length);
    if (!stream) {
        throw runtime_error("Could not read string from file.");
    }
    return value;
}

void FileManager::writeWord(ofstream& stream, const Word& word) const {
    char type;
    if (word.getTypeName() == "Phrase") {
        type = 'P';
    } else {
        type = 'V';
    }

    int id = word.getId();
    int reviewCount = word.getReviewCount();
    time_t nextReviewDate = word.getNextReviewDate();

    stream.write(&type, sizeof(type));
    stream.write(reinterpret_cast<const char*>(&id), sizeof(id));
    writeString(stream, word.getTerm());
    writeString(stream, word.getPartOfSpeech());
    writeString(stream, word.getMeaning());
    stream.write(reinterpret_cast<const char*>(&reviewCount),
                 sizeof(reviewCount));
    stream.write(reinterpret_cast<const char*>(&nextReviewDate),
                 sizeof(nextReviewDate));

    if (!stream) {
        throw runtime_error("Could not write word to file.");
    }
}

unique_ptr<Word> FileManager::readWord(ifstream& stream) const {
    char type;
    int id;
    int reviewCount;
    time_t nextReviewDate;

    stream.read(&type, sizeof(type));
    stream.read(reinterpret_cast<char*>(&id), sizeof(id));
    string term = readString(stream);
    string partOfSpeech = readString(stream);
    string meaning = readString(stream);
    stream.read(reinterpret_cast<char*>(&reviewCount), sizeof(reviewCount));
    stream.read(reinterpret_cast<char*>(&nextReviewDate),
                sizeof(nextReviewDate));

    if (!stream) {
        throw runtime_error("Could not read word from file.");
    }

    unique_ptr<Word> word;
    if (type == 'P') {
        word = make_unique<PhraseWord>(id, term, partOfSpeech, meaning);
    } else if (type == 'V') {
        word = make_unique<VocabularyWord>(id, term, partOfSpeech, meaning);
    } else {
        throw runtime_error("Unknown word type in file.");
    }

    word->setReviewState(reviewCount, nextReviewDate);
    return word;
}

void FileManager::save(const Deck& deck, const string& path) const {
    ofstream stream(path, ios::binary);
    if (!stream) {
        throw runtime_error("Could not open file for writing: " + path);
    }

    uint32_t wordCount = static_cast<uint32_t>(deck.size());
    stream.write(reinterpret_cast<const char*>(&MAGIC), sizeof(MAGIC));
    stream.write(reinterpret_cast<const char*>(&VERSION), sizeof(VERSION));
    stream.write(reinterpret_cast<const char*>(&wordCount), sizeof(wordCount));

    for (const auto& word : deck.all()) {
        writeWord(stream, *word);
    }

    if (!stream) {
        throw runtime_error("Could not save vocabulary file.");
    }
}

void FileManager::load(Deck& deck, const string& path) const {
    ifstream stream(path, ios::binary);
    if (!stream) {
        throw runtime_error("Could not open file for reading: " + path);
    }

    uint32_t magic;
    uint16_t version;
    uint32_t wordCount;

    stream.read(reinterpret_cast<char*>(&magic), sizeof(magic));
    stream.read(reinterpret_cast<char*>(&version), sizeof(version));
    stream.read(reinterpret_cast<char*>(&wordCount), sizeof(wordCount));
    if (!stream || magic != MAGIC || version != VERSION) {
        throw runtime_error("Invalid or damaged vocabulary file.");
    }

    Deck loadedDeck;
    for (uint32_t i = 0; i < wordCount; ++i) {
        loadedDeck.adopt(readWord(stream));
    }

    deck = std::move(loadedDeck);
}

bool FileManager::exists(const string& path) const {
    ifstream stream(path, ios::binary);
    return stream.good();
}
