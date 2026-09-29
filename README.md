# PRF193 Final Project — Flashcard / Vocabulary Manager

## 1. Tổng quan môn học
- Môn: **PRF193 – Programming Fundamentals (C/C++)**
- Project là **dự án nhóm duy nhất**, xây dần qua 5 Lab (không phải 5 project riêng lẻ)
- Đánh giá: Labs 20% + Final Project Evaluation 20% + Practical Exam 30% + Progress Test 10% + Theoretical Exam 20%
- Final Project Evaluation: mỗi nhóm thuyết trình 30 phút + Q&A trước hội đồng (≥2 GV chấm), chấm theo rubric dựa trên final report + demo + Q&A

## 2. CLO cần đáp ứng
| CLO | Nội dung |
|---|---|
| CLO1 | Cú pháp cơ bản C/C++: kiểu dữ liệu, biến, toán tử, string, array, pointer |
| CLO2 | Cấu trúc điều khiển: if-else, switch, for/while/do-while, break/continue/goto |
| CLO3 | Hàm: định nghĩa/khai báo, modular, đệ quy, function pointer, truyền tham số |
| CLO4 | OOP: encapsulation, inheritance, polymorphism; con trỏ động/smart pointer; tránh memory leak; constructor/destructor, template, exception |
| CLO5 | File handling: đọc/ghi text & binary, random access |
| CLO6 | Dùng AI Tools & Computational Thinking để phân tích/mô hình hoá/giải quyết vấn đề — **cần hỏi lại giảng viên: có bắt buộc gọi API AI thật trong code hay chỉ cần thể hiện qua quá trình làm bài (dùng ChatGPT/Copilot hỗ trợ)** |

## 3. Ý tưởng đề tài: Flashcard / Vocabulary Manager
Ứng dụng console quản lý từ vựng — chọn vì có đủ chỗ tự nhiên cho OOP (kế thừa loại từ), file I/O (lưu bộ từ), và tích hợp AI (sinh câu ví dụ) để thoả CLO6.

### Class dự kiến
- `Word` (lớp cha): term, meaning, reviewCount, nextReviewDate
- `VocabularyWord`, `PhraseWord` (kế thừa từ `Word`) — override hàm `getExampleSentence()` → polymorphism
- `Deck`: quản lý danh sách `Word` (dùng `std::vector<Word*>` hoặc `std::map<string, Word>`)

### Chức năng chính
- Add / Edit / Delete từ vựng
- Search theo từ khoá hoặc nghĩa
- List / Sort (alphabet, mức độ nhớ)
- Đánh dấu "đã học" + lên lịch ôn lại (spaced repetition đơn giản)
- Sinh câu ví dụ tự động bằng AI (CLO6)
- Import/Export danh sách từ (CSV)

## 4. Lộ trình theo Lab
| Lab | Tuần | Nội dung chuẩn bị |
|---|---|---|
| Lab 1 | 2 | Phân tích chức năng — mô tả input/output từng chức năng ở mục 3 |
| Lab 2 | 4 | Flowchart cho Add Word, Search Word, luồng Spaced Repetition |
| Lab 3 | 6 (trước PT1) | Code **thủ tục, chưa OOP**: `struct Word`, mảng/con trỏ, hàm addWord/searchWord/sortWords/printAll |
| Lab 4 | 8 | Refactor sang OOP: class + kế thừa + polymorphism, STL (`vector`/`map`), exception handling, smart pointer |
| Lab 5 | 9 | Đọc/ghi file (text/binary), Import/Export CSV, viết test case cho từng chức năng |

## 5. Công cụ & thư viện
**Bắt buộc**
- IDE: Visual Studio Community / VS Code + MinGW / Code::Blocks / CLion
- Compiler hỗ trợ C++11 trở lên

**Thư viện chuẩn (STL)**
- `<string>`, `<vector>` / `<map>`, `<algorithm>` (sort/find)
- `<fstream>`, `<sstream>` — đọc/ghi file
- `<memory>` — `unique_ptr`/`shared_ptr`
- `<stdexcept>` / `<exception>`

**Quản lý code nhóm**
- Git + GitHub/GitLab

**Nếu tích hợp AI thật (tuỳ theo câu trả lời của giảng viên về CLO6)**
- `libcurl` hoặc `cpp-httplib` — gửi HTTP request
- `nlohmann/json` — parse JSON response
- API key OpenAI/Gemini/Anthropic (free tier đủ demo)

## 6. Kiến trúc thư mục dự án

### Giai đoạn Lab 3 (chưa OOP — kiểu thủ tục)
```
FlashcardProject/
├── main.cpp          # toàn bộ logic: struct Word, mảng, các hàm add/search/sort/print
└── README.md
```
Giai đoạn này cố tình để phẳng (không tách file) vì chưa học class — tách quá sớm sẽ khó merge lại đúng chuẩn OOP ở Lab4.

### Giai đoạn cuối (sau Lab 4-5 — OOP + STL + File)
```
FlashcardProject/
├── README.md
├── CMakeLists.txt              # hoặc Makefile / file .sln nếu dùng Visual Studio
├── .gitignore
├── docs/
│   ├── lab1_function_analysis.md
│   ├── lab2_flowcharts/         # ảnh/pdf flowchart từ Lab2
│   └── final_report.md         # báo cáo nộp cho buổi bảo vệ
├── include/                     # header (.h) — khai báo class
│   ├── Word.h
│   ├── VocabularyWord.h         # kế thừa từ Word
│   ├── PhraseWord.h             # kế thừa từ Word
│   ├── Deck.h                   # quản lý vector<Word*>/map<string,Word>
│   ├── FileManager.h            # đọc/ghi file (Lab5)
│   └── AIService.h              # gọi API sinh câu ví dụ (nếu CLO6 yêu cầu gọi API thật)
├── src/                          # implementation (.cpp)
│   ├── main.cpp                 # entry point, menu điều khiển
│   ├── Word.cpp
│   ├── VocabularyWord.cpp
│   ├── PhraseWord.cpp
│   ├── Deck.cpp
│   ├── FileManager.cpp
│   └── AIService.cpp
├── data/
│   ├── words.csv                # dữ liệu dạng text
│   └── words.bin                # dữ liệu dạng binary (demo random access, CLO5)
├── tests/
│   └── test_cases.cpp           # test case cho add/search/sort/spaced repetition (Lab5)
└── lib/                          # thư viện ngoài header-only nếu có
    ├── json.hpp                  # nlohmann/json (nếu gọi AI API)
    └── httplib.h                 # cpp-httplib (nếu gọi AI API)
```

### Giải thích từng folder/file

| Đường dẫn | Vai trò |
|---|---|
| `README.md` | Mô tả tổng quan project, hướng dẫn build/chạy |
| `CMakeLists.txt` / `Makefile` / `.sln` | File cấu hình build — biên dịch toàn bộ `src/` thành 1 executable |
| `.gitignore` | Loại trừ file build (`.exe`, `.o`, thư mục `build/`) khỏi Git |
| `docs/lab1_function_analysis.md` | Bản phân tích chức năng nộp cho Lab1 |
| `docs/lab2_flowcharts/` | Ảnh/PDF flowchart vẽ ở Lab2 (Add Word, Search, Spaced Repetition) |
| `docs/final_report.md` | Báo cáo cuối kỳ dùng khi bảo vệ Final Project |
| `include/Word.h` | Khai báo class `Word` (lớp cha): thuộc tính term/meaning/reviewCount, các hàm ảo (virtual) cho polymorphism |
| `include/VocabularyWord.h` | Khai báo class kế thừa `Word`, override cách sinh câu ví dụ cho từ vựng đơn |
| `include/PhraseWord.h` | Khai báo class kế thừa `Word`, override cách sinh câu ví dụ cho cụm từ |
| `include/Deck.h` | Khai báo class `Deck`: quản lý tập hợp `Word` (add/remove/search/sort), dùng STL container |
| `include/FileManager.h` | Khai báo các hàm đọc/ghi file (text & binary) cho `Deck` |
| `include/AIService.h` | Khai báo hàm gọi API AI sinh câu ví dụ (chỉ cần nếu CLO6 yêu cầu gọi API thật) |
| `src/main.cpp` | Điểm vào chương trình — hiển thị menu, gọi hàm từ các class, không chứa logic nghiệp vụ |
| `src/Word.cpp` | Cài đặt các hàm của class `Word` |
| `src/VocabularyWord.cpp` | Cài đặt class `VocabularyWord` |
| `src/PhraseWord.cpp` | Cài đặt class `PhraseWord` |
| `src/Deck.cpp` | Cài đặt logic quản lý danh sách từ (add/search/sort/spaced repetition) |
| `src/FileManager.cpp` | Cài đặt đọc/ghi `words.csv`/`words.bin`, xử lý import/export (Lab5) |
| `src/AIService.cpp` | Cài đặt gửi HTTP request tới API AI và parse kết quả JSON |
| `data/words.csv` | Dữ liệu từ vựng lưu dạng text, dễ đọc/sửa tay |
| `data/words.bin` | Dữ liệu lưu dạng binary, dùng demo random access file (CLO5) |
| `tests/test_cases.cpp` | Test case kiểm chứng add/search/sort/spaced repetition hoạt động đúng (Lab5) |
| `lib/json.hpp` | Thư viện nlohmann/json (header-only) để parse JSON trả về từ AI API |
| `lib/httplib.h` | Thư viện cpp-httplib (header-only) để gửi HTTP request tới AI API |

 mỗi class 1 cặp `.h`/`.cpp`, `main.cpp` chỉ chứa menu + gọi hàm từ các class — không nhét logic nghiệp vụ vào `main.cpp` để dễ chia việc trong nhóm (mỗi người phụ trách 1-2 class) và để Practical Exam luyện tách file quen tay.

## 7. Việc cần làm ngay
- [x] Chốt đội nhóm [Đặng Đức Duy, Phạm Xuân Dũng, Nguyễn Văn Quân]
- [x] Khởi tạo repo theo kiến trúc thư mục ở mục 6 [Duy did at 29/09/2026]
- [ ] Hỏi giảng viên cách hiểu CLO6 (bắt buộc gọi API AI hay không)
- [ ] Hoàn thiện mô tả chức năng chi tiết cho Lab 1
- [ ] Phân công vai trò trong nhóm theo từng Lab