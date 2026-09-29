# PRF193 Final Project — Flashcard / Vocabulary Manager

Ứng dụng console quản lý từ vựng cho môn **PRF193 – Programming Fundamentals (C/C++)**.

## Thành viên

- Duy
- Dũng
- Quân

## Tổng quan

Đây là dự án nhóm được phát triển qua 5 Lab, tập trung vào quản lý từ vựng và hỗ trợ học bằng phương pháp spaced repetition.

## Chức năng chính

- Thêm, sửa và xoá từ vựng
- Tìm kiếm theo từ khoá hoặc nghĩa
- Liệt kê và sắp xếp theo alphabet hoặc mức độ ghi nhớ
- Đánh dấu từ đã học và lên lịch ôn lại
- Sinh câu ví dụ bằng AI, tuỳ yêu cầu CLO6
- Import/Export danh sách từ bằng CSV

## Thiết kế dự kiến

- `Word`: lớp cơ sở lưu term, meaning, review count và next review date
- `VocabularyWord`, `PhraseWord`: kế thừa `Word` và override `getExampleSentence()`
- `Deck`: quản lý danh sách từ vựng

Dự án áp dụng OOP, kế thừa, polymorphism, STL, smart pointer, exception handling và file handling.

## Lộ trình phát triển

| Lab | Nội dung |
|---|---|
| Lab 1 | Phân tích chức năng và mô tả input/output |
| Lab 2 | Flowchart cho Add Word, Search Word và Spaced Repetition |
| Lab 3 | Phiên bản thủ tục với `struct`, mảng/con trỏ và các hàm xử lý |
| Lab 4 | Refactor sang OOP, STL, kế thừa, polymorphism và exception |
| Lab 5 | Đọc/ghi file, Import/Export CSV và viết test case |

## Công cụ và thư viện

- C++11 trở lên
- Visual Studio, VS Code + MinGW, Code::Blocks hoặc CLion
- Git và GitHub/GitLab
- Thư viện chuẩn: `<string>`, `<vector>`, `<map>`, `<algorithm>`, `<fstream>`, `<sstream>`, `<memory>`, `<exception>`
- Nếu tích hợp AI: `libcurl` hoặc `cpp-httplib`, `nlohmann/json` và API phù hợp

## Việc cần làm

- [ ] Hoàn thiện mô tả chức năng cho Lab 1
- [ ] Hỏi giảng viên về yêu cầu tích hợp AI trong CLO6
- [ ] Phân công vai trò theo từng Lab
- [ ] Thiết kế và triển khai chương trình
