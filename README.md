# Flashcard-Vocabulary Manager

Ứng dụng console viết bằng C++ giúp người học xây dựng bộ flashcard từ vựng và ôn tập theo lịch. Mỗi thẻ lưu từ hoặc cụm từ, từ loại, nghĩa và trạng thái ôn tập.

> **Trạng thái:** README mô tả sản phẩm mục tiêu. Mã nguồn hiện tại vẫn đang trong quá trình phát triển; không phải tất cả chức năng bên dưới đã được triển khai hoặc kiểm thử.

## Sản phẩm hướng tới

Người dùng có thể tạo và quản lý một bộ từ vựng cá nhân ngay trong terminal. Ứng dụng hướng đến việc giúp việc học từ mới có tổ chức hơn: dễ thêm nội dung, tra cứu nhanh, theo dõi những từ cần ôn và giữ lại dữ liệu giữa các lần sử dụng.

## Chức năng dự kiến

- **Quản lý flashcard:** thêm, chỉnh sửa và xóa từ hoặc cụm từ. Mỗi thẻ gồm nội dung, từ loại và nghĩa.
- **Tìm kiếm:** tìm thẻ theo từ/cụm từ hoặc nghĩa.
- **Xem và sắp xếp:** xem toàn bộ bộ thẻ, sắp xếp theo bảng chữ cái hoặc trạng thái ôn tập.
- **Ôn tập:** đưa ra các thẻ đến hạn dưới dạng trắc nghiệm với tối đa ba đáp án gây nhiễu lấy ngẫu nhiên từ Deck. Lịch lặp có năm mốc ngày có thể chỉnh trong menu và được lưu tại `data/review_intervals.csv`.
- **Trao đổi dữ liệu:** nhập và xuất danh sách từ bằng CSV.
- **Lưu trữ:** tự động nạp và lưu bộ thẻ trong `data/words.csv` để dữ liệu còn sau khi thoát ứng dụng.

## Cách sử dụng dự kiến

1. Mở ứng dụng và chọn một thao tác từ menu console.
2. Tạo hoặc nhập các thẻ từ vựng.
3. Tìm kiếm, xem danh sách hoặc bắt đầu phiên ôn tập.
4. Xuất bộ thẻ ra CSV khi cần sao lưu hoặc trao đổi.
5. Dữ liệu được lưu để sử dụng trong lần chạy tiếp theo.

## Thiết kế tổng quan

Ứng dụng được tổ chức quanh các thành phần chính:

- `Word` biểu diễn dữ liệu chung của một thẻ; `VocabularyWord` và `PhraseWord` dự kiến phân biệt từ đơn với cụm từ.
- `Deck` quản lý danh sách thẻ.
- `SearchService`, `WordLister` và `ReviewScheduler` lần lượt phụ trách tìm kiếm, trình bày/sắp xếp và lịch ôn.
- `CsvManager` phụ trách trao đổi CSV; `FileManager` lưu/nạp bộ thẻ ở `data/words.csv`.
- `Menu` là giao diện console, nhận lựa chọn và hiển thị kết quả cho người dùng.

Mục tiêu là tách giao diện, dữ liệu và nghiệp vụ để các chức năng có thể phát triển và kiểm thử độc lập.

## Cấu trúc dự án

```text
Flashcard_project/
├── CMakeLists.txt        # Cấu hình build CMake
├── README.md
├── data/                 # Dữ liệu mẫu và dữ liệu ứng dụng
├── docs/                 # Tài liệu thiết kế, phân tích và kiểm thử
├── include/              # Header của các lớp và thành phần
├── src/                  # Mã nguồn C++
└── tests/                # Mã nguồn kiểm thử
```
## Công nghệ

- C++
- C++17
- STL, CMake và các thư viện chuẩn của C++

## Build và chạy

Mở PowerShell tại thư mục gốc của dự án (`Flashcard_project`) và chạy:

```powershell
$sources = Get-ChildItem .\src\*.cpp | ForEach-Object { $_.FullName }
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude $sources -o .\Flashcard-Vocabulary-Manager.exe
.\Flashcard-Vocabulary-Manager.exe
```

Lệnh đầu tiên biên dịch tất cả file `.cpp` trong `src` thành `Flashcard-Vocabulary-Manager.exe`; lệnh tiếp theo chạy ứng dụng. Cần cài GCC/G++ và đảm bảo `g++` có trong `PATH`.

Trên Windows, ứng dụng đặt console input/output ở UTF-8 để hiển thị và nhập nghĩa tiếng Việt; nên dùng terminal hỗ trợ Unicode.

Menu hiện hỗ trợ quản lý, tìm kiếm, liệt kê, ôn tập trắc nghiệm, tùy chỉnh lịch lặp, CSV import/export và tự động lưu/nạp dữ liệu. Kết quả tìm kiếm hiển thị đầy đủ thông tin thẻ; sửa hoặc xóa cần tìm kiếm từ khóa trước và chọn ID trong kết quả.

## Tài liệu dự án

- [`docs/design_decisions.md`](docs/design_decisions.md) — thiết kế và quyết định kiến trúc.
- [`docs/lab1_function_analysis.md`](docs/lab1_function_analysis.md) — phân tích luồng chức năng dự kiến.
- [`docs/lab2_flowcharts/`](docs/lab2_flowcharts/) — sơ đồ luồng.
- [`docs/test_report.md`](docs/test_report.md) — tài liệu kiểm thử.
- [`docs/final_report.md`](docs/final_report.md) — báo cáo dự án.

Các tài liệu về môn học, quá trình làm nhóm và sử dụng AI được giữ trong `docs/` để README tập trung vào mục đích và cách hiểu sản phẩm.
