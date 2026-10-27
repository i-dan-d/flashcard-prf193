cmake_minimum_required(VERSION 3.16)

project(Flashcard-Vocabulary-Manager LANGUAGES CXX)

include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(flashcard_vocabulary_manager_core
	src/Deck.cpp
	src/FileManager.cpp
	src/Input.cpp
	src/Menu.cpp
	src/Menu_AddEditDelete.cpp
	src/Menu_ImportExport.cpp
	src/Menu_List.cpp
	src/Menu_Review.cpp
	src/Menu_Search.cpp
	src/PhraseWord.cpp
	src/ReviewScheduler.cpp
	src/StringUtil.cpp
	src/VocabularyWord.cpp
	src/Word.cpp
	src/WordLister.cpp
)

target_include_directories(flashcard_vocabulary_manager_core PUBLIC include)

add_executable(Flashcard-Vocabulary-Manager src/main.cpp)
target_link_libraries(Flashcard-Vocabulary-Manager PRIVATE flashcard_vocabulary_manager_core)

if(BUILD_TESTING)
	add_executable(test_word_lister tests/test_word_lister.cpp)
	target_link_libraries(test_word_lister PRIVATE flashcard_vocabulary_manager_core)
	add_test(NAME word_lister COMMAND test_word_lister)
endif()
