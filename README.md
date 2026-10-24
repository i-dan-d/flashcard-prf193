# PRF193 Final Project — Flashcard / Vocabulary Manager

Nhóm: **BetterMan** | Thành viên: Duy, Quân, Dũng | Leader: phụ trách merge vào `main` và tích hợp tài liệu
Cập nhật lần cuối: 24/10/2026 | Hạn cuối của nhóm: **01/11/2026**

---

## 1. Dự án là gì
Ứng dụng console bằng C++ để quản lý từ vựng: thêm/sửa/xoá từ, tìm kiếm, liệt kê và sắp xếp, ôn tập theo lịch lặp lại ngắt quãng (spaced repetition), nhập/xuất CSV, và tự lưu dữ liệu giữa các lần chạy.
Đề tài đủ chỗ tự nhiên cho OOP (kế thừa loại từ), STL, exception và file I/O.

## 2. Yêu cầu của môn (PRF193)

| Đầu điểm | Trọng số | Ghi chú |
|---|---|---|
| Labs (5 bài) | 20% | Chấm theo rubric Computational Thinking, xem mục 9 |
| Final Project Evaluation | 20% | Bảo vệ 30 phút (thuyết trình + Q&A), hội đồng ≥ 2 giảng viên; căn cứ final report + demo + Q&A; rubric chưa có |
| Practical Exam | 30% | Tự viết code, 85 phút |
| Progress Test (2 bài) | 10% | |
| Theoretical Exam | 20% | |

| CLO | Nội dung | Thể hiện trong dự án ở đâu |
|---|---|---|
| CLO1 | Cú pháp, kiểu dữ liệu, string, con trỏ | Toàn bộ code, `StringUtil` |
| CLO2 | Cấu trúc điều khiển | Vòng lặp menu, Review, Search |
| CLO3 | Hàm, modular | Mỗi chức năng là hàm/method riêng |
| CLO4 | OOP, con trỏ động, exception | `Word` và lớp con, `Deck`, `unique_ptr`, `try-catch` |
| CLO5 | File text và binary | `CsvManager` (text), `FileManager` (binary) |
| CLO6 | AI Tools và Computational Thinking | **Đã xác nhận với giảng viên:** không cần gọi AI API trong code; chỉ cần ghi lại đã dùng AI để làm gì và làm như thế nào trong quá trình làm dự án → `docs/ai_usage_log.md` |

## 3. Chức năng và phân công

| Mã | Chức năng | Người | Mục trong Lab1 |
|---|---|---|---|
| CN1 | Add / Edit / Delete từ vựng | Duy | 1, 2 |
| CN2 | Search theo từ khoá hoặc nghĩa | Quân | 3 |
| CN3 | List / Sort (alphabet, mức độ nhớ) | Dũng | 4 |
| CN4 | Đánh dấu "đã học" + lên lịch ôn lại (spaced repetition đơn giản) | Duy | 5 |
| CN5 | Import / Export danh sách từ (CSV) | Duy | 6, 7 |
| CN6 | Save / Load `words.bin` (tự động khi mở và thoát chương trình) | **Chưa giao** (đề xuất Dũng) | 8 |

Dùng mã CN1–CN6 trong commit, tên ca test và mục báo cáo.

## 4. Kiến trúc lớp

### 4.1. Cây kế thừa
```
Word (abstract)
├── VocabularyWord      term một từ
└── PhraseWord          term nhiều từ
```
Các lớp còn lại không kế thừa mà liên kết với nhau:
- `Menu` **sở hữu** `Deck` và các service. Trong `Menu`, `Deck` phải khai báo trước các service, vì chúng giữ tham chiếu tới `Deck`.
- `Deck` **chứa** `vector<unique_ptr<Word>>`.
- `SearchService`, `WordLister` giữ `const Deck&` (chỉ đọc); `ReviewScheduler` giữ `Deck&` (sửa lịch ôn).
- `CsvManager`, `FileManager` nhận `Deck` qua tham số hàm.
- Mọi mũi tên đi từ `Menu` xuống `Word`, không có vòng phụ thuộc.

### 4.2. Các lớp

| Lớp | Vai trò | Người | Private (chính) | Public (chính) |
|---|---|---|---|---|
| `Word` (abstract) | Dữ liệu một từ | Duy | `idWord`, `term`, `partOfSpeech`, `meaning`, `reviewCount`, `nextReviewDate` | getter/setter, `setReviewState()`, `getTypeName() = 0`, `static create()` |
| `VocabularyWord`, `PhraseWord` | Hai loại từ | Duy | (không có) | constructor, `getTypeName()` override |
| `StringUtil` | Hàm chuỗi dùng chung | Duy | (không có) | `toLower`, `trim`, `isBlank`, `equalsIgnoreCase`, `containsIgnoreCase` (đều static) |
| `Deck` | Danh sách Word và thao tác lõi | Duy | `vector<unique_ptr<Word>> words`, `nextId` | `addWord`, `editWord`, `removeById`, `findById`, `findByTerm`, `adopt`, `all`, `size`, `empty`, `clear` |
| `SearchService` | Tìm theo term hoặc meaning | Quân | `const Deck&`, `matches()` | `search(keyword)` |
| `WordLister` | Sắp xếp và định dạng | Dũng | `const Deck&`, `lessByAlphabet`, `lessByReviewCount` | `SortKey`, `sortedBy(key)`, `format(word)` |
| `ReviewScheduler` | Lịch ôn lặp lại | Duy | `Deck&`, `INTERVAL_DAYS`, `addDays()` | `dueWords`, `nextDue`, `checkAnswer`, `markResult` |
| `CsvManager` | Import/Export CSV | Duy | `escapeField`, `parseLine` | `importFrom`, `exportTo` (ném `runtime_error` khi lỗi file) |
| `FileManager` | Save/Load `words.bin` | Chưa giao | `MAGIC`, `VERSION`, `writeString`, `readString`, `writeWord`, `readWord` | `save`, `load`, `exists` |
| `Menu` | Giao diện duy nhất | Leader | `deck`, `searcher`, `lister`, `scheduler`, `csv`, `fileManager`, `dataPath`, các `handle...()`, `readChoice`, `loadOnStart`, `saveOnExit` | `Menu()`, `run()` |
| `Input` *(đề xuất)* | Hàm nhập dùng chung | Leader | (không có) | `readNonEmpty`, `readInt`, `askContinue` (đều static) |

Quy tắc kèm theo: hàm trả kết quả, **việc in nằm ở `Menu`**; `Deck::adopt()` cập nhật `nextId = max(nextId, id + 1)` để từ thêm mới sau khi nạp file không trùng `IdWord`.

### 4.3. Luồng chạy
`main()` → `Menu::run()` → `loadOnStart()` → vòng lặp (hiện menu, đọc lựa chọn, gọi handler, handler gọi `Deck` hoặc service rồi in kết quả) → khi chọn Thoát thì `saveOnExit()`.

Menu chính: 1 Quản lý từ (menu con Add/Edit/Delete), 2 Tìm kiếm, 3 Danh sách và sắp xếp, 4 Ôn tập, 5 Import/Export, 0 Thoát. Save/Load chạy tự động nên không có mục menu.

## 5. Cấu trúc thư mục

```
FlashcardProject/
├── README.md
├── Makefile                     # hoặc CMakeLists.txt; build mọi src/*.cpp, không gõ tay từng file
├── .gitignore                   # build/, *.o, *.exe, data/words.bin
├── docs/
│   ├── lab1_function_analysis.md
│   ├── lab2_flowcharts/         # hình flowchart xuất từ Lab2
│   ├── lab_rubric_checklist.md
│   ├── final_project_evaluation.md
│   ├── project_timeline.md
│   ├── design_decisions.md      # (chưa tạo) nguồn sự thật cho mọi quyết định thiết kế
│   ├── test_report.md           # (chưa tạo) output chạy thật của test
│   ├── ai_usage_log.md          # nhật ký dùng AI (CLO6)
│   └── final_report.md          # (chưa tạo) báo cáo nộp
├── include/
│   ├── Word.h  VocabularyWord.h  PhraseWord.h
│   ├── StringUtil.h  Input.h    # Input.h: đề xuất
│   ├── Deck.h
│   ├── SearchService.h  WordLister.h  ReviewScheduler.h
│   ├── CsvManager.h  FileManager.h
│   └── Menu.h
├── src/
│   ├── main.cpp                 # chỉ vài dòng: Menu menu; menu.run();
│   ├── Word.cpp  VocabularyWord.cpp  PhraseWord.cpp
│   ├── StringUtil.cpp  Input.cpp
│   ├── Deck.cpp
│   ├── SearchService.cpp  WordLister.cpp  ReviewScheduler.cpp
│   ├── CsvManager.cpp  FileManager.cpp
│   ├── Menu.cpp                 # constructor, run, readChoice, loadOnStart, saveOnExit
│   ├── Menu_AddEditDelete.cpp   # CN1 — Duy
│   ├── Menu_Search.cpp          # CN2 — Quân
│   ├── Menu_List.cpp            # CN3 — Dũng
│   ├── Menu_Review.cpp          # CN4 — Duy
│   └── Menu_ImportExport.cpp    # CN5 — Duy
├── data/
│   ├── words.csv                # dữ liệu mẫu (commit)
│   └── words.bin                # sinh khi chạy (không commit)
└── tests/
    └── test_cases.cpp           # có main() riêng
```

Ba điểm cần nhớ:
- Mỗi class một cặp `.h`/`.cpp`. Hàm thành viên của `Menu` được viết rải ở nhiều file `Menu_*.cpp`, mỗi file `#include "Menu.h"`. Mỗi người sửa file của mình nên ít conflict.
- `main.cpp` chỉ khởi tạo `Menu`. Không đặt logic nghiệp vụ ở đây.
- Khi build test, biên dịch mọi file trong `src/` **trừ `main.cpp`** cùng `tests/test_cases.cpp`, nếu không sẽ báo trùng `main`.

## 6. Công cụ và build
- Compiler: g++ (MSYS2 UCRT64 hoặc tương đương), chuẩn **C++17** (tối thiểu C++11 để dùng `unique_ptr`).
- Build chương trình: `g++ -std=c++17 -Wall -Wextra -Iinclude <mọi file src/*.cpp> -o build/app`. Dùng Makefile hoặc CMake để lấy danh sách file; ký tự `*` có thể không tự mở rộng tuỳ shell (PowerShell). Lệnh này chưa được chạy thử trên máy các bạn.
- Code phải biên dịch sạch với `-Wall -Wextra`.
- Thư viện: chỉ STL (`<string>`, `<vector>`, `<algorithm>`, `<memory>`, `<fstream>`, `<sstream>`, `<stdexcept>`, `<ctime>`). Không cần thư viện bên ngoài.
- Quản lý mã nguồn: Git + GitHub.

## 7. Làm việc nhóm
- Mỗi người một nhánh `dev/<tên>`; không ai push thẳng vào `main`; **chỉ leader merge**.
- Commit nhỏ, mở đầu bằng mã chức năng ("CN2: ..."). Mỗi chức năng xong thì leader merge ngay, không gom cuối.
- Trước khi merge, leader xem `git log main..origin/dev/x` và `git diff main...origin/dev/x`, merge bằng `--no-ff`, build sạch và chạy test rồi mới push `main`.
- Chức năng "xong" khi: build sạch, có bằng chứng chạy thật, comment ghi bước tương ứng trong Lab1, đã push, đã báo leader.
- Mỗi lần dùng AI ghi vào `docs/ai_usage_log.md`: tự gõ trước, dùng AI để giải thích hoặc review.
- Chi tiết 22 quy cách: file `Quy_cach_lam_viec_nhom.csv`.

## 8. Quyết định thiết kế

| Nội dung | Trạng thái |
|---|---|
| Sáu chức năng CN1–CN6, người phụ trách CN1–CN5 | Đã chốt |
| Mỗi người một nhánh, leader merge | Đã chốt |
| Add: không phân biệt hoa/thường khi kiểm tra trùng; `type` tự xác định; có `partOfSpeech`; vòng hỏi tiếp tục `c`/`q` | Đã chốt (theo `lab1_function_analysis.md`) |
| Edit và Delete gộp chung một chức năng | Đã chốt |
| CLO6 chỉ cần nhật ký dùng AI, không gọi API | Đã chốt (giảng viên xác nhận) |
| `type` tự xác định: term có khoảng trắng → `PhraseWord`, ngược lại `VocabularyWord` | Đề xuất, chờ chốt |
| Lịch ôn: đúng thì `reviewCount + 1`, dời lịch theo 1, 3, 7, 15 ngày; sai thì `reviewCount = 0`, ôn lại hôm sau | Đề xuất, chờ chốt |
| Định dạng CSV: `term,partOfSpeech,meaning` | Đề xuất, chờ chốt |
| Bản ghi `words.bin`: tag loại (1 byte), id (int32), reviewCount (int32), nextReviewDate (int64), 3 chuỗi có độ dài đứng trước (uint32); ghi file tạm rồi đổi tên | Đề xuất, chờ chốt |
| Thêm lớp `Input` cho hàm nhập dùng chung | Đề xuất, chờ chốt |
| `Word` có hàm ảo `matchesAnswer()` với hành vi khác nhau giữa hai lớp con; tách `WordFactory` khỏi `Word::create` | Đề xuất, chờ chốt |
| Người nhận CN6 | Chưa giao |

Các quyết định "đề xuất" phải được ghi vào `docs/design_decisions.md` khi chốt.

## 9. Lab và rubric

Nội dung Lab theo syllabus (cần giảng viên xác nhận, xem bên dưới):

| Lab | Tuần | Nội dung |
|---|---|---|
| Lab 1 | 2 | Phân tích chức năng của project |
| Lab 2 | 4 | Thiết kế bằng flowchart |
| Lab 3 | 6 | Cài đặt (trước Progress Test 1) |
| Lab 4 | 8 | OOP và STL |
| Lab 5 | 9 | File và Testing |

Rubric Lab (mỗi Lab cùng 4 tiêu chí): Decomposition 30%, Pattern Recognition 20%, Abstraction 20%, Algorithmic Thinking 30%. Chi tiết trong `docs/lab_rubric_checklist.md`.

**Chưa khớp, đang chờ giảng viên:** rubric Lab đặt chủ đề khác (I/O, điều kiện, vòng lặp, mảng/cấu trúc dữ liệu, hàm + OOP) và thưởng dùng STL từ sớm. Vì hạn của nhóm là 01/11, dự án xây thẳng kiến trúc cuối (OOP + STL), không làm bản thủ tục rồi refactor.

## 10. Việc cần làm ngay
- [ ] Giao người nhận CN6 (Save/Load)
- [ ] Chốt các mục "đề xuất" ở mục 8 và tạo `docs/design_decisions.md`
- [ ] Leader viết bộ khung (khai báo + thân hàm rỗng) của mọi lớp, kể cả `Menu`, `Input`, rồi đẩy lên `main`; mọi người rẽ nhánh từ đó
- [ ] Hỏi giảng viên: Lab thực tế làm gì, thang điểm rubric Lab, được dùng STL sớm không, rubric Final Project, chấm cá nhân hay chấm nhóm
- [ ] Lập lại timeline theo ngày 24/10 cho các thành viên
- [ ] Sửa `lab1_function_analysis.md` (định dạng CSV, thêm cột Processing) và vẽ lại flowchart cho khớp
- [ ] Bắt đầu ghi `docs/ai_usage_log.md`