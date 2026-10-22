===============1===============
- Class: Word (abstract, dùng chung) — người giữ: Duy (1)
  - Private: int idWord; std::string term; std::string partOfSpeech; std::string meaning;
             int reviewCount; std::time_t nextReviewDate;
  - Public:  Word(int id, term, partOfSpeech, meaning); virtual ~Word();
             getId() / getTerm() / getPartOfSpeech() / getMeaning() / getReviewCount() / getNextReviewDate() const;
             setTerm() / setPartOfSpeech() / setMeaning(); setReviewState(int count, std::time_t next);
             virtual std::string getTypeName() const = 0;
             static std::unique_ptr<Word> create(id, term, partOfSpeech, meaning);
               // tự xác định type: term có khoảng trắng → PhraseWord, ngược lại VocabularyWord (quy tắc đề xuất)

- Class: VocabularyWord : Word — Duy (1)
  - Private: (không có)
  - Public:  VocabularyWord(id, term, partOfSpeech, meaning); getTypeName() override → "Vocabulary"

- Class: PhraseWord : Word — Duy (1)
  - Private: (không có)
  - Public:  PhraseWord(id, term, partOfSpeech, meaning); getTypeName() override → "Phrase"

- Class: StringUtil (dùng chung, toàn hàm static) — Duy (1)
  - Private: (không có)
  - Public:  static toLower(s); trim(s); isBlank(s); equalsIgnoreCase(a, b); containsIgnoreCase(text, key)

- Class: Deck — Duy (1)
  - Private: std::vector<std::unique_ptr<Word>> words; int nextId;
  - Public:  bool addWord(term, partOfSpeech, meaning);          // false nếu trùng term (không phân biệt hoa/thường)
             bool editWord(int id, newTerm, newPartOfSpeech, newMeaning);   // cũng kiểm tra trùng với từ khác
             bool removeById(int id);
             Word* findById(int id); Word* findByTerm(const std::string& term) const;
             void adopt(std::unique_ptr<Word> w);               // nạp Word dựng sẵn (cho FileManager)
             const std::vector<std::unique_ptr<Word>>& all() const; size_t size() const; bool empty() const; void clear();
===============2===============
- Class: SearchService — Quân (2)
  - Private: const Deck& deck; bool matches(const Word& w, const std::string& keyLower) const;
  - Public:  explicit SearchService(const Deck& deck);
             std::vector<const Word*> search(const std::string& keyword) const;
               // khớp term hoặc meaning, không phân biệt hoa/thường; dùng std::copy_if
===============3===============
- Class: WordLister — Dũng (3)
  - Private: const Deck& deck; static bool lessByAlphabet(const Word*, const Word*);
             static bool lessByReviewCount(const Word*, const Word*);
  - Public:  enum class SortKey { Alphabet, ReviewCount };
             explicit WordLister(const Deck& deck);
             std::vector<const Word*> sortedBy(SortKey key) const;   // std::sort + comparator; Deck rỗng → trả vector rỗng
             static std::string format(const Word& w);               // trả chuỗi, việc in nằm ở Menu
===============4===============
- Class: ReviewScheduler — Duy (4)
  - Private: Deck& deck; static constexpr int INTERVAL_DAYS[] = {1, 3, 7, 15};
             std::time_t addDays(std::time_t t, int days) const;
  - Public:  explicit ReviewScheduler(Deck& deck);
             std::vector<Word*> dueWords(std::time_t now) const;     // nextReviewDate <= now, sắp theo hạn sớm nhất
             Word* nextDue(std::time_t now) const;                   // nullptr nếu hết từ
             bool checkAnswer(const Word& w, const std::string& answer) const;   // so với meaning, không phân biệt hoa/thường
             void markResult(Word& w, bool correct, std::time_t now);
               // đúng: reviewCount+1, dời lịch theo INTERVAL_DAYS; sai: reviewCount=0, ôn lại hôm sau
===============5===============
- Class: CsvManager — Duy (5)
  - Private: static std::string escapeField(const std::string& s);
             static bool parseLine(const std::string& line, std::string& term, std::string& partOfSpeech, std::string& meaning);
  - Public:  int importFrom(const std::string& path, Deck& deck, int& skippedLines) const;
               // ném std::runtime_error nếu không mở được file; dòng sai định dạng thì bỏ qua và đếm
             void exportTo(const std::string& path, const Deck& deck) const;   // ném std::runtime_error nếu không ghi được

- Class: Menu (giao diện duy nhất; main() chỉ gọi Menu().run()) — người giữ: leader
  - Private: Deck deck; SearchService searcher; WordLister lister; ReviewScheduler scheduler; CsvManager csv;
             void handleAddEditDelete();  // Duy (1)       — viết trong Menu_AddEditDelete.cpp
             void handleSearch();         // Quân (2)      — viết trong Menu_Search.cpp
             void handleList();           // Dũng (3)      — viết trong Menu_List.cpp
             void handleReview();         // Duy (4)       — viết trong Menu_Review.cpp
             void handleImportExport();   // Duy (5)       — viết trong Menu_ImportExport.cpp
             int readChoice() const;
  - Public:  Menu(); void run();

- Class: FileManager (ĐỀ XUẤT BỔ SUNG, chưa giao ai) — Save/Load words.bin
  - Private: static constexpr std::uint32_t MAGIC;
  - Public:  void save(const Deck& deck, const std::string& path) const;
             void load(Deck& deck, const std::string& path) const;   // ném std::runtime_error nếu file hỏng; dùng Deck::adopt
===============6===============
- Class: FileManager (6) — người giữ: chưa giao
  - Private: static constexpr std::uint32_t MAGIC;           // nhận diện đúng file của app
             static constexpr std::uint16_t VERSION = 1;
             void writeString(std::ofstream&, const std::string&) const;   // độ dài (uint32) + nội dung
             std::string readString(std::ifstream&) const;
             void writeWord(std::ofstream&, const Word&) const;
             std::unique_ptr<Word> readWord(std::ifstream&) const;         // đọc tag loại rồi dựng đúng lớp con
  - Public:  void save(const Deck& deck, const std::string& path) const;
               // ghi header + số bản ghi + từng Word; ghi ra file tạm rồi đổi tên để không hỏng file cũ nếu mất điện giữa chừng
             void load(Deck& deck, const std::string& path) const;
               // ném std::runtime_error nếu file không mở được, sai MAGIC hoặc hỏng; dựng Word xong gọi Deck::adopt
             bool exists(const std::string& path) const;

- Class: Menu (phần thay đổi so với bản trước)
  - Private: thêm FileManager fileManager; const std::string dataPath = "data/words.bin";
             void loadOnStart();   // file chưa có: bỏ qua, Deck rỗng; file hỏng: cảnh báo, Deck rỗng, không crash
             void saveOnExit();    // lưu khi chọn Thoát (có thể thêm mục "Lưu ngay" trong menu)
  - Public:  void run();  // loadOnStart() → vòng lặp menu → saveOnExit()

- Class: Deck (phần thay đổi)
  - Public:  adopt(std::unique_ptr<Word> w) cập nhật nextId = max(nextId, w->getId() + 1)
             // nếu không, từ thêm mới sau khi nạp file có thể trùng IdWord