# Lab 1 — Function Analysis: Flashcard / Vocabulary Manager

M�n: PRF193 | Nhóm: BetterMan

## Mục đích tài liệu
Phân tích chi tiết luồng xử lý, input/output, điều kiện, và ngoại lệ cho từng chức năng chính của ứng dụng, làm cơ sở cho Lab 2 (flowchart) và Lab 3-5 (code).

> **CLO6 (AI Tools):** đã xác nhận với giảng viên — không cần code gọi AI API trong chương trình. CLO6 thể hiện qua `docs/ai_usage_log.md`, ghi lại quá trình nhóm dùng AI hỗ trợ làm dự án, không phải một chức năng của app nên không có mục riêng trong tài liệu này.

---

## 1. Add Word
**Mô tả:** Thêm một từ vựng hoặc câu mới vào Deck
**Tiền điều kiện:** Deck đã được nạp
**Hậu điều kiện:** Deck có thêm 1 Word mới (nếu thành công)

**Luồng chính:**
1. Người dùng chọn Add Word từ menu
2. Hệ thống yêu cầu nhập `term`, `partOfSpeech`, `meaning`
3. Hệ thống kiểm tra `term` đã tồn tại chưa (không phân biệt hoa/thường)
4. Nếu chưa tồn tại → tự động xác định `type` cho `term`, tạo object `Word` (gán `IdWord`), thêm vào Deck, lưu file
5. Hệ thống hỏi `progress` ('c' = tiếp tục / 'q' = thoát)
6. Nếu 'c' → quay lại bước 2; nếu 'q' → kết thúc, về menu

**Luồng rẽ nhánh:**
- Tại bước 2: nếu `term`/`meaning` rỗng → từ chối, yêu cầu nhập lại
- Tại bước 3: nếu `term` đã tồn tại → báo lỗi trùng, quay lại bước 2
- Tại bước 5: nếu `progress` khác 'c'/'q' → báo lỗi, hỏi lại

| Input | Output |
|---|---|
| term, partOfSpeech, meaning | Word mới (type tự xác định) + thông báo kết quả |
| progress ('c'/'q') | Điều hướng: quay lại bước 2 hoặc kết thúc |

**CLO liên quan:** CLO1 (string), CLO4 (OOP)

---

## 2. Edit Word
**Mô tả:** Chỉnh sửa hoặc xoá một Word đã có trong Deck
**Tiền điều kiện:** Term muốn chỉnh sửa phải tồn tại trong Deck
**Hậu điều kiện:** Deck cập nhật dữ liệu đã chỉnh sửa, hoặc Word bị xoá khỏi Deck

**Luồng chính:**
1. Người dùng chọn Edit Word từ menu
2. Hệ thống yêu cầu nhập `keyword`
3. Hệ thống gọi chức năng **Search Word**, trả về word list khớp với `keyword`
4. Hệ thống yêu cầu nhập `term` cần chỉnh sửa
5. Hệ thống kiểm tra `term` có thuộc word list vừa tìm được không
6. Nếu có → cho chọn hành động: edit `meaning` / edit `term` / xoá Word
7. Hệ thống cập nhật (hoặc xoá) đúng Word dựa vào `IdWord` & `term`

**Luồng rẽ nhánh:**
- Tại bước 5: nếu `term` không thuộc word list → hỏi người dùng muốn quay lại bước 2 (tìm `keyword` mới) hay quay lại bước 4 (nhập lại `term`)

| Input | Output |
|---|---|
| keyword | Word list trả về từ chức năng Search Word |
| term | Kết quả kiểm tra `term` có thuộc word list không |
| progress ('w'/'t') | 'w' → quay lại bước 2; 't' → quay lại bước 4 |

**CLO liên quan:** CLO4 (truy cập/sửa đổi object qua con trỏ/reference)

---

## 3. Search Word
**Mô tả:** Tra từ vựng hoặc câu tương ứng trong Deck
**Tiền điều kiện:** Deck đã được nạp
**Hậu điều kiện:** Deck giữ nguyên dữ liệu (không thay đổi)

**Luồng chính:**
1. Người dùng chọn Search Word từ menu
2. Hệ thống yêu cầu nhập `key` để tìm kiếm
3. Hệ thống kiểm tra `key` có khớp (trùng hoặc tương đồng, không phân biệt hoa/thường) với Word nào trong Deck không
4. Nếu tìm thấy → hiển thị kết quả → trả về danh sách Word tìm được → kết thúc

**Luồng rẽ nhánh:**
- Tại bước 3: nếu `key` không khớp Word nào → báo không tìm thấy → kết thúc

| Input | Output |
|---|---|
| key | Danh sách các Word tương đồng với `key` |

**CLO liên quan:** CLO1-2 (string, điều kiện), CLO4 (STL `find`/`algorithm`)

---

## 4. List All & Sort
**Mô tả:** Hiển thị toàn bộ Deck, sắp xếp theo alphabet hoặc theo mức độ nhớ (`reviewCount`)
**Tiền điều kiện:** Deck đã được nạp (có thể rỗng)
**Hậu điều kiện:** Deck giữ nguyên dữ liệu, chỉ hiển thị

**Luồng chính:**
1. Người dùng chọn List & Sort từ menu
2. Hệ thống hỏi tiêu chí sắp xếp (alphabet / reviewCount)
3. Hệ thống kiểm tra Deck có rỗng không
4. Nếu không rỗng → sắp xếp theo tiêu chí đã chọn → hiển thị danh sách → kết thúc

**Luồng rẽ nhánh:**
- Tại bước 3: nếu Deck rỗng → thông báo "Deck trống" → kết thúc (không gọi sort)

| Input | Output |
|---|---|
| Tiêu chí sắp xếp (alphabet / reviewCount) | Danh sách Word đã sắp xếp, hoặc thông báo Deck trống |

**CLO liên quan:** CLO4 (STL `algorithm::sort` với comparator tuỳ chỉnh)

---

## 5. Review (Spaced Repetition)
**Mô tả:** Người dùng ôn từ bằng phương pháp lặp lại ngắt quãng
**Tiền điều kiện:** Có Word đến hạn ôn tại thời điểm hiện tại (nếu có)
**Hậu điều kiện:** Deck cập nhật lịch ôn (`nextReviewDate`, `reviewCount`) của từng Word dựa trên câu trả lời đúng/sai

**Luồng chính:**
1. Người dùng chọn Study Word từ menu
2. Hệ thống lọc các Word đến hạn ôn hôm nay
3. Hệ thống kiểm tra còn Word nào cần ôn trong Deck không
4. Nếu còn → chọn Word có hạn ôn sớm nhất → hiển thị `term`, nhận `answer` từ người dùng (không phân biệt hoa/thường)
5. Hệ thống đối chiếu `answer` với `meaning`, cập nhật lịch ôn theo kết quả đúng/sai → quay lại bước 3

**Luồng rẽ nhánh:**
- Tại bước 3: nếu không còn Word nào đến hạn → báo không có từ cần ôn → kết thúc

| Input | Output |
|---|---|
| answer | Đối chiếu với `meaning` của `term` đang hỏi; Deck cập nhật lịch ôn tương ứng |

**CLO liên quan:** CLO2 (vòng lặp, điều kiện), CLO4 (OOP, cập nhật state object)

---

## 6. Import Words (từ CSV)
**Mô tả:** Đọc danh sách từ từ file `.csv`, thêm vào Deck
**Tiền điều kiện:** File CSV tồn tại, đúng định dạng (`term,meaning,type`)
**Hậu điều kiện:** Deck được nạp thêm các Word mới hợp lệ

**Luồng chính:**
1. Người dùng chọn Import từ menu
2. Hệ thống yêu cầu nhập đường dẫn file CSV
3. Hệ thống kiểm tra file có mở được không
4. Nếu mở được → đọc từng dòng, parse `term`/`meaning`/`type`, thêm vào Deck → báo số dòng đọc thành công/lỗi

**Luồng rẽ nhánh:**
- Tại bước 3: file không tồn tại/không mở được → báo lỗi → kết thúc
- Tại bước 4: dòng dữ liệu sai định dạng → bỏ qua dòng đó, tiếp tục đọc dòng kế tiếp

| Input | Output |
|---|---|
| Đường dẫn file CSV | Deck được cập nhật; số dòng đọc thành công/lỗi |

**CLO liên quan:** CLO5 (file handling), CLO4 (exception)

---

## 7. Export Words (ra CSV)
**Mô tả:** Ghi toàn bộ Deck hiện tại ra file `.csv`
**Tiền điều kiện:** Deck đã được nạp (có thể rỗng)
**Hậu điều kiện:** File CSV chứa dữ liệu Deck tại thời điểm export

**Luồng chính:**
1. Người dùng chọn Export từ menu
2. Hệ thống yêu cầu nhập đường dẫn file đích
3. Hệ thống ghi từng Word trong Deck ra file theo định dạng CSV → báo thành công

**Luồng rẽ nhánh:**
- Tại bước 3: không ghi được file (quyền truy cập, ổ đĩa đầy) → báo lỗi, không crash

| Input | Output |
|---|---|
| Đường dẫn file đích | File CSV chứa toàn bộ dữ liệu Deck |

**CLO liên quan:** CLO5 (file handling)

---

## 8. Save/Load (Auto khi mở/đóng chương trình)
**Mô tả:** Tự động lưu Deck ra `words.bin` (binary) khi thoát, tự động nạp lại khi mở chương trình
**Tiền điều kiện:** Không bắt buộc có sẵn file `words.bin` (nếu chưa có → khởi tạo Deck rỗng)
**Hậu điều kiện:** Dữ liệu được giữ nguyên giữa các lần chạy chương trình

**Luồng chính:**
1. Khi mở chương trình → hệ thống kiểm tra file `words.bin` có tồn tại không
2. Nếu có → đọc và nạp vào Deck; nếu không → khởi tạo Deck rỗng
3. Khi người dùng thoát chương trình → hệ thống ghi Deck hiện tại ra `words.bin`

**Luồng rẽ nhánh:**
- Tại bước 2: file `words.bin` bị hỏng/sai định dạng → báo cảnh báo, khởi tạo Deck rỗng (không crash)

| Input | Output |
|---|---|
| (không cần người dùng thao tác — chạy ngầm) | Dữ liệu Deck được giữ nguyên giữa các lần chạy |

**CLO liên quan:** CLO5 (random access binary file)

---

## Bảng tổng hợp CLO coverage

| CLO | Các chức năng liên quan |
|---|---|
| CLO1 (cú pháp cơ bản) | Add Word, Search Word |
| CLO2 (điều khiển) | Search Word, Review |
| CLO3 (hàm) | Toàn bộ — mỗi chức năng là 1 hàm/method |
| CLO4 (OOP, con trỏ, exception) | Add, Edit, Search, List & Sort, Review, Import |
| CLO5 (file) | Import, Export, Save/Load |
| CLO6 (AI Tools) | *Không phải 1 chức năng của app* — thể hiện qua `docs/ai_usage_log.md` |

→ 5 CLO đầu đều có ít nhất 1 chức năng phủ tới trong app; CLO6 chứng minh qua nhật ký sử dụng AI, nộp kèm khi bảo vệ.