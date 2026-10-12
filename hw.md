Bài này là “Exercise 1: Wrap a C driver” — tức là:

Viết một lớp C++ để bọc API C trong mock_dec.h và mock_dec.c
Không dùng heap / không cấp phát động bên trong wrapper
Chuyển quyền sở hữu tài nguyên (context) sang class C++
Dùng RAII: constructor khởi tạo, destructor giải phóng
Yêu cầu cụ thể
Tạo một class wrapper cho dec_ctx_t

Gọi dec_init() khi object được tạo
Gọi dec_close() khi object bị hủy
Quy tắc ownership

Không cho copy
Có thể move nếu cần
Không để copy tạo ra nhiều object cùng quản lý cùng context
Kiểm tra lỗi

Nếu key == 0 thì khởi tạo phải fail
Nếu ctx chưa khởi tạo hoặc đã đóng thì feed()/close() phải báo lỗi đúng
Không được “đóng một object không hợp lệ” mà không ghi nhận
Mục tiêu kỹ thuật

Không có new/delete
Không leak
init_count == close_count sau khi scope kết thúc
bad_close_count phải đúng
static_assert(!std::is_copy_constructible_v<Decoder>) phải pass




cd \mock_example 1

g++ -std=c++17 -Wall -Wextra -pedantic test_decoder_wrapper.cpp mock_dec.c -o test_decoder_wrapper.exe

.\test_decoder_wrapper.exe