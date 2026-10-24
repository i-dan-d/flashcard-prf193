# Flashcard Vocabulary Manager

Ứng dụng console viết bằng C++ giúp người học xây dựng bộ flashcard từ vựng và ôn tập theo lịch. Mỗi thẻ lưu từ hoặc cụm từ, từ loại, nghĩa và trạng thái ôn tập.

> **Trạng thái:** README mô tả sản phẩm mục tiêu. Mã nguồn hiện tại vẫn đang trong quá trình phát triển; không phải tất cả chức năng bên dưới đã được triển khai hoặc kiểm thử.

## Sản phẩm hướng tới

Người dùng có thể tạo và quản lý một bộ từ vựng cá nhân ngay trong terminal. Ứng dụng hướng đến việc giúp việc học từ mới có tổ chức hơn: dễ thêm nội dung, tra cứu nhanh, theo dõi những từ cần ôn và giữ lại dữ liệu giữa các lần sử dụng.

## Chức năng dự kiến

- **Quản lý flashcard:** thêm, chỉnh sửa và xóa từ hoặc cụm từ. Mỗi thẻ gồm nội dung, từ loại và nghĩa.
- **Tìm kiếm:** tìm thẻ theo từ/cụm từ hoặc nghĩa.
- **Xem và sắp xếp:** xem toàn bộ bộ thẻ, sắp xếp theo bảng chữ cái hoặc trạng thái ôn tập.
- **Ôn tập:** đưa ra các thẻ đến hạn, nhận câu trả lời của người học và cập nhật lịch ôn tiếp theo. Lịch dự kiến tăng dần khoảng cách giữa các lần ôn khi trả lời đúng; trả lời sai sẽ đưa thẻ trở lại sớm hơn.
- **Trao đổi dữ liệu:** nhập và xuất danh sách từ bằng CSV.
- **Lưu trữ:** tự động nạp và lưu bộ thẻ trong file nhị phân để dữ liệu còn sau khi thoát ứng dụng.

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
- `CsvManager` và `FileManager` phụ trách trao đổi CSV và lưu trữ nhị phân.
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
- C++17 là chuẩn dự kiến
- STL, CMake và các thư viện chuẩn của C++

## Build và chạy

Quy trình build/run chưa sẵn sàng để hướng dẫn đáng tin cậy: `CMakeLists.txt` hiện chưa có cấu hình target, và mã nguồn trong `src/` chưa tạo thành phiên bản ứng dụng hoàn chỉnh. Các lệnh build và chạy sẽ được bổ sung sau khi cấu hình build và entry point của ứng dụng được hoàn thiện, rồi kiểm chứng.

## Tài liệu dự án

- [`docs/design_decisions.md`](docs/design_decisions.md) — thiết kế và quyết định kiến trúc.
- [`docs/lab1_function_analysis.md`](docs/lab1_function_analysis.md) — phân tích luồng chức năng dự kiến.
- [`docs/lab2_flowcharts/`](docs/lab2_flowcharts/) — sơ đồ luồng.
- [`docs/test_report.md`](docs/test_report.md) — tài liệu kiểm thử.
- [`docs/final_report.md`](docs/final_report.md) — báo cáo dự án.

Các tài liệu về môn học, quá trình làm nhóm và sử dụng AI được giữ trong `docs/` để README tập trung vào mục đích và cách hiểu sản phẩm.
