# Flashcard-Vocabulary Manager

Ứng dụng console viết bằng C++ giúp người học xây dựng bộ flashcard từ vựng và ôn tập theo lịch. Mỗi thẻ lưu từ hoặc cụm từ, từ loại, nghĩa và trạng thái ôn tập.

> **Trạng thái:** README này mô tả các chức năng hiện có trong phiên bản dự án.

## Sản phẩm hướng tới

Người dùng có thể tạo và quản lý một bộ từ vựng cá nhân ngay trong terminal. Ứng dụng hướng đến việc giúp việc học từ mới có tổ chức hơn: dễ thêm nội dung, tra cứu nhanh, theo dõi những từ cần ôn và giữ lại dữ liệu giữa các lần sử dụng.

## Chức năng

- **Quản lý flashcard:** thêm, chỉnh sửa và xóa từ hoặc cụm từ. Mỗi thẻ gồm ID nội bộ, nội dung, từ loại, nghĩa và trạng thái ôn tập.
- **Tìm kiếm:** tìm thẻ theo từ/cụm từ hoặc nghĩa.
- **Xem và sắp xếp:** xem toàn bộ bộ thẻ, sắp xếp theo bảng chữ cái hoặc trạng thái ôn tập.
- **Ôn tập:** đưa ra các thẻ đến hạn dưới dạng trắc nghiệm với tối đa ba đáp án gây nhiễu lấy ngẫu nhiên từ Deck. Lịch lặp có năm mốc ngày có thể chỉnh trong menu và được lưu tại `data/review_intervals.csv`.
- **Trao đổi dữ liệu:** nhập và xuất CSV có tiêu đề cột; ngày ôn tiếp theo dùng định dạng `dd/MM/yyyy` để dễ đọc và chỉnh sửa.
- **Lưu trữ:** tự động nạp và lưu bộ thẻ trong `data/words.csv` để dữ liệu còn sau khi thoát ứng dụng.

## Cách sử dụng

1. Mở ứng dụng và chọn một thao tác từ menu console.
2. Tạo hoặc nhập các thẻ từ vựng.
3. Tìm kiếm, xem danh sách hoặc bắt đầu phiên ôn tập.
4. Xuất bộ thẻ ra CSV khi cần sao lưu hoặc trao đổi.
5. Dữ liệu được lưu để sử dụng trong lần chạy tiếp theo.

## Thiết kế tổng quan

Ứng dụng được tổ chức quanh các thành phần chính:

- `Word` biểu diễn dữ liệu chung của một thẻ; `VocabularyWord` và `PhraseWord` phân biệt từ đơn với cụm từ.
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

Project dùng CMake để cấu hình và build ứng dụng cùng các test; CTest chạy
các test đã đăng ký trong `CMakeLists.txt`. CMake tạo quy trình build, còn
CTest không tự tạo test — mỗi test cần kiểm tra kết quả và trả mã thoát khác
0 khi thất bại.

Cài CMake và một compiler C++17 (ví dụ GCC/G++ hoặc Visual Studio), rồi mở
PowerShell tại thư mục gốc của dự án (`Flashcard_project`):

```powershell
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
.\build\Flashcard-Vocabulary-Manager.exe
```

`cmake -S . -B build` cấu hình project và sinh build files trong `build`;
`cmake --build build` biên dịch ứng dụng và test; `ctest` chạy test và hiển
thị lỗi nếu có. Có thể build riêng ứng dụng với
`cmake --build build --target Flashcard-Vocabulary-Manager`, hoặc chỉ build
các test với `cmake --build build --target test_cases test_word_lister`.

Trên Windows, ứng dụng đặt console input/output ở UTF-8 để hiển thị và nhập nghĩa tiếng Việt; nên dùng terminal hỗ trợ Unicode.

Menu hiện hỗ trợ quản lý, tìm kiếm, liệt kê, ôn tập trắc nghiệm, tùy chỉnh lịch lặp, CSV import/export và tự động lưu/nạp dữ liệu. Kết quả tìm kiếm hiển thị đầy đủ thông tin thẻ; sửa hoặc xóa cần tìm kiếm từ khóa trước và chọn ID trong kết quả.

## Tài liệu dự án

- [`docs/design_decisions.md`](docs/design_decisions.md) — thiết kế và quyết định kiến trúc.
- [`docs/lab1_function_analysis.md`](docs/lab1_function_analysis.md) — phân tích luồng chức năng dự kiến.
- [`docs/lab2_flowcharts/`](docs/lab2_flowcharts/) — sơ đồ luồng.
- [`docs/test_report.md`](docs/test_report.md) — tài liệu kiểm thử.
- [`docs/final_report.md`](docs/final_report.md) — báo cáo dự án.

Các tài liệu về môn học, quá trình làm nhóm và sử dụng AI được giữ trong `docs/` để README tập trung vào mục đích và cách hiểu sản phẩm.
