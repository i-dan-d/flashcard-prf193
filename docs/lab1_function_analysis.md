# Lab 1 — Function Analysis: Flashcard / Vocabulary Manager

Môn: PRF193 | Nhóm: BetterMan
## Mục đích tài liệu
Phân tích chi tiết input/output, điều kiện, và ngoại lệ cho từng chức năng chính của ứng dụng, làm cơ sở cho Lab 2 (flowchart) và Lab 3-5 (code).

---

### 1. Add Word
**Mô tả:** Thêm một từ vựng hoặc câu mới vào Deck
**Tiền điều kiện:** Deck đã được nạp
**Hậu điều kiện:** Deck có thêm 1 Word mới (nếu thành công)

**Luồng chính**:
1. Người dùng chọn Add Word từ menu
2. Hệ thống yêu cầu nhập `term`, `partOfSpeech`, `meaning`
3. Hệ thống kiểm tra `term` đã tồn tại chưa (không phân biệt hoa/thường)
4. Nếu chưa tồn tại → tự động xác định `type` cho `term`, tạo object `Word`, thêm vào Deck, lưu file
5. Hệ thống hỏi `progress` ('c' = tiếp tục / 'q' = thoát)
6. Nếu 'c' → quay lại bước 2; nếu 'q' → kết thúc, về menu

**Luồng rẽ nhánh:**
- Tại bước 2: nếu `term`/`meaning` rỗng → từ chối, yêu cầu nhập lại
- Tại bước 3: nếu `term` đã tồn tại → báo lỗi trùng, quay lại bước 2
- Tại bước 5: nếu `progress` khác 'c'/'q' → báo lỗi, hỏi lại

| Input | Output |
|---|---|
| term, partOfSpeech, meaning, type | Word mới + thông báo kết quả |
| progress ('c'/'q') | Điều hướng: quay lại bước 2 hoặc kết thúc |

**CLO liên quan:** CLO1 (string), CLO4 (OOP)

## 2. Edit Word

| Mục | Nội dung |
|---|---|
| Mô tả | Sửa `meaning` hoặc `type` của một từ đã có |
| Input | `term` cần sửa, giá trị mới cho `meaning`/`type` |
| Output | Từ được cập nhật trong Deck; thông báo thành công |
| Điều kiện tiên quyết | `term` phải tồn tại trong Deck |
| Ngoại lệ | Không tìm thấy `term` → báo lỗi "không tồn tại", không thực hiện sửa |
| CLO liên quan | CLO4 (truy cập/sửa đổi object qua con trỏ/reference) |

## 3. Delete Word

| Mục | Nội dung |
|---|---|
| Mô tả | Xoá một từ khỏi Deck |
| Input | `term` cần xoá |
| Output | Từ bị xoá khỏi Deck; thông báo thành công |
| Điều kiện tiên quyết | `term` phải tồn tại |
| Ngoại lệ | Không tìm thấy `term` → báo lỗi, không xoá; cần giải phóng bộ nhớ đúng cách nếu Word được cấp phát động (tránh memory leak) |
| CLO liên quan | CLO4 (quản lý bộ nhớ động) |

## 4. Search Word
**Mô tả:** Tra từ vựng hoặc câu tương ứng trong Deck
**Tiền điều kiện:** Tra được từ vựng hoặc câu trong Deck (nếu word đó tồn tại trong deck)
**Hậu điều kiện:** Deck vẫn giữ nguyên dữ liệu

**Luồng chính**:
1. Người dùng chọn Search Word từ menu
2. Hệ thống yêu cầu người dùng nhập `key` để tìm kiếm Word
3. Hệ thống kiểm tra `key` đấy có trong word nào trong Deck không(Không phân biệt hoa/thường).
4. Hệ thống kiểm tra xem `key` đấy có tương đồng với Word nào trong Deck không.
5. Nếu tìm thấy -> Hiển thị kết quả -> Kết thúc -> Trả về danh sách các word tìm kiếm được trong Deck

**Luồng rẽ nhánh:**
- Tại bước 3: nếu `key` không có trong Deck → báo cáo không có trong Deck -> Kết thúc

| Input | Output |
|---|---|
| key | Các Word có tính tương đầu với key |

**CLO liên quan:** CLO1-2 (string, điều kiện), STL `find`/`algorithm` (CLO4)

## 5. List All & Sort

| Mục | Nội dung |
|---|---|
| Mô tả | Hiển thị toàn bộ Deck, sắp xếp theo alphabet hoặc theo mức độ nhớ (`reviewCount`) |
| Input | Tiêu chí sắp xếp do người dùng chọn (alphabet / reviewCount) |
| Output | Danh sách từ đã sắp xếp, in ra màn hình |
| Điều kiện tiên quyết | Deck không rỗng (nếu rỗng → thông báo "Deck trống") |
| Ngoại lệ | Deck rỗng → không gọi `sort`, tránh lỗi truy cập ngoài phạm vi |
| CLO liên quan | CLO4 (STL `algorithm::sort` với comparator tuỳ chỉnh) |

## 6. Review (Spaced Repetition)

| Mục | Nội dung |
|---|---|
| Mô tả | Lọc các từ có `nextReviewDate <= hôm nay`, hiển thị lần lượt, nhận phản hồi đúng/sai, cập nhật lại lịch ôn |
| Input | Phản hồi đúng/sai của người dùng cho mỗi từ được hỏi |
| Output | `reviewCount` và `nextReviewDate` của từ được cập nhật; thông báo hoàn thành phiên ôn khi hết từ |
| Điều kiện tiên quyết | Có ít nhất 1 từ đến hạn ôn hôm nay |
| Ngoại lệ | Không có từ nào đến hạn → thông báo "không có từ cần ôn hôm nay", không vào vòng lặp review |
| CLO liên quan | CLO2 (vòng lặp, điều kiện), CLO4 (OOP, cập nhật state object) |

## 7. Generate Example Sentence (AI)

| Mục | Nội dung |
|---|---|
| Mô tả | Gọi AI API để sinh câu ví dụ cho một từ, lưu vào field `exampleSentence` của `Word` |
| Input | `term`, `meaning` của từ |
| Output | Câu ví dụ do AI trả về, được lưu vào object `Word` |
| Điều kiện tiên quyết | Có kết nối mạng, có API key hợp lệ |
| Ngoại lệ | Mất mạng/API lỗi/timeout → dùng câu ví dụ mặc định (fallback), không để chương trình crash; cần `try-catch` quanh lời gọi API |
| CLO liên quan | **CLO6** (AI Tools), CLO4 (exception handling) — *lưu ý: cần xác nhận với giảng viên xem CLO6 có bắt buộc gọi API thật hay không (xem mục README)* |

## 8. Import Words (từ CSV)

| Mục | Nội dung |
|---|---|
| Mô tả | Đọc danh sách từ từ file `.csv`, thêm vào Deck |
| Input | Đường dẫn file CSV |
| Output | Deck được nạp thêm các từ mới; báo số dòng đọc thành công/lỗi |
| Điều kiện tiên quyết | File tồn tại, đúng định dạng (term,meaning,type) |
| Ngoại lệ | File không tồn tại/không mở được → báo lỗi, không crash; dòng dữ liệu sai định dạng → bỏ qua dòng đó, tiếp tục đọc dòng sau |
| CLO liên quan | CLO5 (file handling), CLO4 (exception) |

## 9. Export Words (ra CSV)

| Mục | Nội dung |
|---|---|
| Mô tả | Ghi toàn bộ Deck hiện tại ra file `.csv` |
| Input | Đường dẫn file đích |
| Output | File CSV chứa toàn bộ dữ liệu Deck |
| Điều kiện tiên quyết | Deck không rỗng (nếu rỗng vẫn cho xuất file header-only, không bắt buộc là lỗi) |
| Ngoại lệ | Không ghi được file (quyền truy cập, ổ đĩa đầy) → báo lỗi, không crash |
| CLO liên quan | CLO5 (file handling) |

## 10. Save/Load (Auto khi mở/đóng chương trình)

| Mục | Nội dung |
|---|---|
| Mô tả | Tự động lưu Deck ra `words.bin` (binary) khi thoát chương trình, tự động nạp lại khi mở chương trình |
| Input | Không cần người dùng thao tác — chạy ngầm |
| Output | Dữ liệu được giữ nguyên giữa các lần chạy chương trình |
| Điều kiện tiên quyết | File `words.bin` đúng định dạng đã lưu trước đó (nếu chưa có file → tạo Deck rỗng) |
| Ngoại lệ | File `words.bin` bị hỏng/sai định dạng → không crash, khởi tạo Deck rỗng và báo cảnh báo |
| CLO liên quan | CLO5 (random access binary file) |

---

## Bảng tổng hợp CLO coverage

| CLO | Các chức năng liên quan |
|---|---|
| CLO1 (cú pháp cơ bản) | Add, Search |
| CLO2 (điều khiển) | Search, Review |
| CLO3 (hàm) | Toàn bộ — mỗi chức năng là 1 hàm/method |
| CLO4 (OOP, con trỏ, exception) | Add, Edit, Delete, List&Sort, Review, Generate Example, Import/Export |
| CLO5 (file) | Import, Export, Save/Load |
| CLO6 (AI Tools) | Generate Example Sentence |

→ Đủ 6 CLO đều có ít nhất 1 chức năng phủ tới — an toàn khi hội đồng hỏi "chức năng nào thể hiện CLO X?".