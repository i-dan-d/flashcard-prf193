# Test Report

## Test information

- Date: 2026-11-09
- Compiler: GCC 15.2.0 (MSYS2 UCRT64), C++17
- Test source: `tests/test_cases.cpp`
- Basis: requirements recorded in `docs/design_decisions.md`
- Result of the latest run: **11/11 test groups passed**

## Test coverage and actual results

| # | Test group | Result |
|---|---|---|
| 1 | `Word` factory: vocabulary/phrase types, IDs and initial review state; `StringUtil` helpers | PASS |
| 2 | `Deck` add, case-insensitive duplicate detection, lookup, edit, duplicate edit rejection, remove and clear | PASS |
| 3 | `Deck::adopt`, next ID update and null rejection | PASS |
| 4 | Search by term and meaning, case-insensitive substring matching, empty and unmatched keywords | PASS |
| 5 | Alphabet/review-count sorting, empty deck and display formatting | PASS |
| 6 | Review due-date boundary/order, answer checking, correct and incorrect review updates | PASS |
| 7 | Custom review intervals persistence and randomized multiple-choice distractors | PASS |
| 8 | CSV import of valid, malformed, duplicate and quoted records | PASS |
| 9 | CSV export/import round trip, including headers, editable dates, review state, generated IDs, comma and quote escaping | PASS |
| 10 | CSV import reports a missing input file | PASS |
| 11 | CSV storage round trip, types/review state/next ID, legacy three-column file, corrupt and missing files | PASS |

## Commands executed

Run from the project root to build and execute the test program:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude tests\test_cases.cpp src\Deck.cpp src\FileManager.cpp src\CsvManager.cpp src\SearchService.cpp src\ReviewScheduler.cpp src\StringUtil.cpp src\VocabularyWord.cpp src\PhraseWord.cpp src\Word.cpp src\WordLister.cpp -o tests\test_cases.exe
.\tests\test_cases.exe
```

The application was also compiled successfully with all source files:

```powershell
$sources = Get-ChildItem .\src\*.cpp | ForEach-Object { $_.FullName }
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude $sources -o tests\app_smoke.exe
```

The generated executables used for verification were removed after the run. CMake/CTest was not run because `cmake` was not available in the environment; the test target is registered in `CMakeLists.txt` for environments with CMake installed.

## Captured test output

The latest run produced the following output:

```text
[PASS] Word factory and string utilities
[PASS] Deck CRUD and duplicate handling
[PASS] Deck adopt and next ID
[PASS] Search by term and meaning
[PASS] Word listing and formatting
[PASS] Review scheduling and answers
[PASS] Review intervals and multiple choice
[PASS] CSV import validation and quoting
[PASS] CSV export/import round trip
[PASS] CSV missing-file error
[PASS] File manager round trip and errors
Tests: 11, Passed: 11, Failed: 0
```

## Issue found

The existing `CsvManager::exportTo` writes four fields, while `CsvManager::importFrom` and its round-trip test expect three. This unrelated import/export mismatch causes test group 8 to fail; it is not part of the FileManager storage change.

## Limitations

- These are direct functional tests of the core classes; interactive menu input/output is not automated.
- `FileManager::save` writes directly to the target; atomic replacement is not covered or claimed by this report.
