# Ghi chú đọc code SHA-256

## 1. File `structure.h`

File này chứa các thành phần nền tảng của SHA-256:

- `H0_INITIAL`: 8 giá trị băm khởi tạo (lấy từ phần phân số của căn bậc hai của 8 số nguyên tố đầu tiên).
- `K256`: 64 hằng số vòng (lấy từ phần phân số của căn bậc ba của 64 số nguyên tố đầu tiên).
- `ROTR`, `SHR`: xoay phải và dịch phải các bit của một word 32-bit.
- `Ch`, `Maj`: hai hàm logic chính trong vòng nén để xáo trộn dữ liệu đầu vào.
- `Sigma0_256`, `Sigma1_256`, `sigma0_256`, `sigma1_256`: các hàm dịch, xoay bit phức hợp dùng để sinh mảng thông điệp rộng W.

## 2. File `sha256_lib.h`

File này gom phần xử lý cốt lõi thành các hàm có thể tái sử dụng:

- `bytes_to_word_be()`: chuyển 4 byte thành 1 word 32-bit định dạng Big Endian (phục vụ tính toán toán học).
- `word_to_bytes_be()`: chuyển ngược 1 word 32-bit thành 4 byte dạng Big Endian để xuất kết quả.
- `sha256_transform()`: xử lý và nén dữ liệu lõi của 1 khối (block) 512 bit thông qua 64 vòng lặp.
- `calculate_sha256_bytes()`: thực hiện đệm (padding) chuẩn Merkle-Damgård, chia khối dữ liệu, gọi hàm transform và trả về mảng kết quả 32 byte dữ liệu băm.
- `calculate_sha256_string()`: hàm tiện ích dùng để băm trực tiếp một chuỗi ký tự.
- `calculate_sha256_file()`: hàm tiện ích dùng để đọc luồng byte của một tệp và băm nội dung tệp đó.

## 3. File `sha_procedure.cpp`

Đây là chương trình chính để sinh viên chạy nhanh SHA-256 từ terminal:

```bash
./sha256 --self-test
./sha256 --hash-string "abc"
./sha256 --hash-file sample.txt
```

## 4. Các bài ứng dụng

- `file_integrity.cpp`: kiểm tra toàn vẹn file bằng cách so sánh hash hiện tại của tệp với chuỗi hash mong muốn được cung cấp.
- `password_hash.cpp`: minh họa cơ chế băm và xác thực mật khẩu thông thường bằng cách lưu trữ chuỗi băm thô.
- `salted_password_hash.cpp`: nâng cao bảo mật bằng cách trộn thêm chuỗi muối (salt) ngẫu nhiên, giúp ngăn chặn việc hai tài khoản trùng mật khẩu tạo ra chuỗi băm giống nhau trên cơ sở dữ liệu.

## 5. Câu trả lời câu hỏi gợi ý cho sinh viên

1. **Vì sao SHA-256 xử lý dữ liệu theo block 512 bit?**
   - Do thiết kế của sơ đồ nén Merkle-Damgård yêu cầu kích thước đầu vào cố định cho mỗi phân đoạn xử lý. Khối 512 bit (tương đương 16 words x 32-bit) là kích thước tối ưu hóa giúp các phép toán dịch/xoay bit chạy hiệu quả trên cấu trúc kiến trúc máy tính và đủ không gian để chèn thêm độ dài thông điệp ở cuối khối padding.

2. **Vì sao kết quả SHA-256 luôn dài 256 bit dù input dài/ngắn khác nhau?**
   - Vì bất kể thông điệp gốc dài bao nhiêu, cơ chế đệm (padding) luôn đưa tổng độ dài thông điệp về bội số của 512 bit. Sau đó, qua các vòng nén liên tục, trạng thái dữ liệu luôn được cô đọng và lưu trữ trong đúng 8 thanh ghi trạng thái 32-bit (H0 đến H7). Kết quả băm cuối cùng là sự nối chuỗi của 8 thanh ghi này, tạo ra giá trị cố định 8 x 32 = 256 bit (hiển thị dưới dạng chuỗi hexa 64 ký tự).

3. **Nếu sửa 1 ký tự trong file, hash thay đổi như thế nào?**
   - Chuỗi băm đầu ra sẽ thay đổi toàn bộ và ngẫu nhiên, không còn bất kỳ sự tương quan nào với chuỗi băm cũ. Đây được gọi là "Hiệu ứng thác đổ" (Avalanche Effect) trong mật mã học, giúp người quản trị dễ dàng phát hiện tệp đã bị chỉnh sửa dù chỉ là một bit nhỏ nhất.

4. **Vì sao không nên lưu mật khẩu plaintext?**
   - Vì nếu cơ sở dữ liệu của hệ thống bị rò rỉ hoặc bị hacker tấn công, toàn bộ mật khẩu dạng rõ (plaintext) của người dùng sẽ bị lộ ngay lập tức. Hacker có thể dùng thông tin này để chiếm đoạt tài khoản hoặc thực hiện tấn công dây chuyền sang các hệ thống/nền tảng lớn khác mà người dùng đặt trùng mật khẩu.

5. **Vì sao chỉ hash mật khẩu bằng SHA-256 vẫn chưa đủ an toàn cho hệ thống thật?**
   - Vì SHA-256 là hàm băm tốc độ cao, được thiết kế để xử lý dữ liệu lớn một cách nhanh chóng. Hacker có thể sử dụng các hệ thống phần cứng mạnh (như GPU hoặc chip ASIC chuyên dụng) để tính toán hàng tỷ phép băm một giây, từ đó thực hiện tấn công vét cạn (brute-force) hoặc dùng bảng băm tra cứu sẵn (Rainbow Tables) để dò ra mật khẩu gốc trong thời gian ngắn. Hệ thống thật cần sử dụng các thuật toán chuyên dụng có cơ chế làm chậm phần cứng và tiêu tốn bộ nhớ như Argon2id, bcrypt hoặc scrypt.