# Design Decisions

## 1. Core classes

### Word (abstract, dùng chung) — người giữ: Duy (1)

| Phạm vi | Thành phần | Ghi chú |
|---|---|---|
| Private | `int idWord`; `std::string term`; `std::string partOfSpeech`; `std::string meaning`; `int reviewCount`; `std::time_t nextReviewDate` | |
| Public | `Word(int id, term, partOfSpeech, meaning)`; `virtual ~Word()` | Hàm khởi tạo và hủy ảo. |
| Public | `getId()`; `getTerm()`; `getPartOfSpeech()`; `getMeaning()`; `getReviewCount()`; `getNextReviewDate()` | Các hàm getter `const`. |
| Public | `setTerm()`; `setPartOfSpeech()`; `setMeaning()`; `setReviewState(int count, std::time_t next)` | Cập nhật thông tin từ và trạng thái ôn tập. |
| Public | `virtual std::string getTypeName() const = 0` | Hàm thuần ảo. |
| Public | `static std::unique_ptr<Word> create(id, term, partOfSpeech, meaning)` | Tự xác định type: `term` có khoảng trắng thì tạo `PhraseWord`, ngược lại tạo `VocabularyWord` (quy tắc đề xuất). |

### VocabularyWord : Word — Duy (1)

| Phạm vi | Thành phần | Ghi chú |
|---|---|---|
| Private | Không có | |
| Public | `VocabularyWord(id, term, partOfSpeech, meaning)` | Hàm khởi tạo. |
| Public | `getTypeName() override` | Trả về `"Vocabulary"`. |

### PhraseWord : Word — Duy (1)

| Phạm vi | Thành phần | Ghi chú |
|---|---|---|
| Private | Không có | |
| Public | `PhraseWord(id, term, partOfSpeech, meaning)` | Hàm khởi tạo. |
| Public | `getTypeName() override` | Trả về `"Phrase"`. |

### StringUtil (dùng chung, toàn hàm static) — Duy (1)

| Phạm vi | Thành phần | Ghi chú |
|---|---|---|
| Private | Không có | |
| Public | `static toLower(s)`; `trim(s)`; `isBlank(s)`; `equalsIgnoreCase(a, b)`; `containsIgnoreCase(text, key)` | Các hàm tiện ích xử lý chuỗi. |

### Deck — Duy (1)

| Phạm vi | Thành phần | Ghi chú |
|---|---|---|
| Private | `std::vector<std::unique_ptr<Word>> words`; `int nextId` | |
| Public | `bool addWord(term, partOfSpeech, meaning)` | Trả về `false` nếu trùng `term` (không phân biệt hoa/thường). |
| Public | `bool editWord(int id, newTerm, newPartOfSpeech, newMeaning)` | Cũng kiểm tra trùng với từ khác. |
| Public | `bool removeById(int id)` | |
| Public | `Word* findById(int id)`; `Word* findByTerm(const std::string& term) const` | |
| Public | `void adopt(std::unique_ptr<Word> w)` | Nạp `Word` dựng sẵn (cho `FileManager`). |
| Public | `const std::vector<std::unique_ptr<Word>>& all() const`; `size_t size() const`; `bool empty() const`; `void clear()` | |

## 2. Search

### SearchService — Quân (2)

| Phạm vi | Thành phần | Ghi chú |
|---|---|---|
| Private | `const Deck& deck`; `bool matches(const Word& w, const std::string& keyLower) const` | |
| Public | `explicit SearchService(const Deck& deck)` | Hàm khởi tạo. |
| Public | `std::vector<const Word*> search(const std::string& keyword) const` | Khớp `term` hoặc `meaning`, không phân biệt hoa/thường; dùng `std::copy_if`. |

## 3. Listing

### WordLister — Dũng (3)

| Phạm vi | Thành phần | Ghi chú |
|---|---|---|
| Private | `const Deck& deck`; `static bool lessByAlphabet(const Word*, const Word*)`; `static bool lessByReviewCount(const Word*, const Word*)` | |
| Public | `enum class SortKey { Alphabet, ReviewCount }` | |
| Public | `explicit WordLister(const Deck& deck)` | Hàm khởi tạo. |
| Public | `std::vector<const Word*> sortedBy(SortKey key) const` | Dùng `std::sort` và comparator; nếu `Deck` rỗng thì trả vector rỗng. |
| Public | `static std::string format(const Word& w)` | Trả chuỗi; việc in nằm ở `Menu`. |

## 4. Review

### ReviewScheduler — Duy (4)

| Phạm vi | Thành phần | Ghi chú |
|---|---|---|
| Private | `Deck& deck`; `static constexpr int INTERVAL_DAYS[] = {1, 3, 7, 15}`; `std::time_t addDays(std::time_t t, int days) const` | |
| Public | `explicit ReviewScheduler(Deck& deck)` | Hàm khởi tạo. |
| Public | `std::vector<Word*> dueWords(std::time_t now) const` | Lọc các từ có `nextReviewDate <= now`, sắp theo hạn sớm nhất. |
| Public | `Word* nextDue(std::time_t now) const` | Trả `nullptr` nếu hết từ. |
| Public | `bool checkAnswer(const Word& w, const std::string& answer) const` | So với `meaning`, không phân biệt hoa/thường. |
| Public | `void markResult(Word& w, bool correct, std::time_t now)` | Đúng: tăng `reviewCount` lên 1 và dời lịch theo `INTERVAL_DAYS`; sai: đặt `reviewCount = 0`, ôn lại hôm sau. |

## 5. CSV và Menu

### CsvManager — Duy (5)

| Phạm vi | Thành phần | Ghi chú |
|---|---|---|
| Private | `static std::string escapeField(const std::string& s)`; `static bool parseLine(const std::string& line, std::string& term, std::string& partOfSpeech, std::string& meaning)` | |
| Public | `int importFrom(const std::string& path, Deck& deck, int& skippedLines) const` | Ném `std::runtime_error` nếu không mở được file; dòng sai định dạng thì bỏ qua và đếm. |
| Public | `void exportTo(const std::string& path, const Deck& deck) const` | Ném `std::runtime_error` nếu không ghi được. |

### Menu (giao diện duy nhất; `main()` chỉ gọi `Menu().run()`) — người giữ: Duy

| Phạm vi | Thành phần | Ghi chú |
|---|---|---|
| Private | `Deck deck`; `SearchService searcher`; `WordLister lister`; `ReviewScheduler scheduler`; `CsvManager csv` | |
| Private | `void handleAddEditDelete()` | Duy (1), viết trong `Menu_AddEditDelete.cpp`. |
| Private | `void handleSearch()` | Quân (2), viết trong `Menu_Search.cpp`. |
| Private | `void handleList()` | Dũng (3), viết trong `Menu_List.cpp`. |
| Private | `void handleReview()` | Duy (4), viết trong `Menu_Review.cpp`. |
| Private | `void handleImportExport()` | Duy (5), viết trong `Menu_ImportExport.cpp`. |
| Private | `int readChoice() const` | |
| Public | `Menu()`; `void run()` | |

### FileManager - Duy — Save/Load `words.bin`

| Phạm vi | Thành phần | Ghi chú |
|---|---|---|
| Private | `static constexpr std::uint32_t MAGIC` | |
| Public | `void save(const Deck& deck, const std::string& path) const` | |
| Public | `void load(Deck& deck, const std::string& path) const` | Ném `std::runtime_error` nếu file hỏng; dùng `Deck::adopt`. |

## 6. File persistence

### FileManager (6) — 

| Phạm vi | Thành phần | Ghi chú |
|---|---|---|
| Private | `static constexpr std::uint32_t MAGIC` | Nhận diện đúng file của app. |
| Private | `static constexpr std::uint16_t VERSION = 1` | |
| Private | `void writeString(std::ofstream&, const std::string&) const` | Ghi độ dài (`uint32`) và nội dung. |
| Private | `std::string readString(std::ifstream&) const` | |
| Private | `void writeWord(std::ofstream&, const Word&) const` | |
| Private | `std::unique_ptr<Word> readWord(std::ifstream&) const` | Đọc tag loại rồi dựng đúng lớp con. |
| Public | `void save(const Deck& deck, const std::string& path) const` | Ghi header, số bản ghi và từng `Word`; ghi ra file tạm rồi đổi tên để không làm hỏng file cũ nếu mất điện giữa chừng. |
| Public | `void load(Deck& deck, const std::string& path) const` | Ném `std::runtime_error` nếu file không mở được, sai `MAGIC` hoặc bị hỏng; dựng `Word` xong gọi `Deck::adopt`. |
| Public | `bool exists(const std::string& path) const` | |

### Menu 

| Phạm vi | Thành phần | Ghi chú |
|---|---|---|
| Private | `FileManager fileManager`; `const std::string dataPath = "data/words.bin"` | Thành phần bổ sung. |
| Private | `void loadOnStart()` | Nếu file chưa có: bỏ qua, `Deck` rỗng; nếu file hỏng: cảnh báo, `Deck` rỗng, không crash. |
| Private | `void saveOnExit()` | Lưu khi chọn Thoát (có thể thêm mục “Lưu ngay” trong menu). |
| Public | `void run()` | `loadOnStart()` → vòng lặp menu → `saveOnExit()`. |

### Deck (phần thay đổi cần bổ sung "Nếu còn thời gian làm")

| Phạm vi | Thành phần | Ghi chú |
|---|---|---|
| Public | `adopt(std::unique_ptr<Word> w)` | Cập nhật `nextId = max(nextId, w->getId() + 1)`. Nếu không, từ thêm mới sau khi nạp file có thể trùng `IdWord`. |