#include "sha256_lib.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>

namespace {

std::string to_lower_hex(std::string text) {
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    return text;
}

void print_usage(const char* program_name) {
    std::cout << "Usage:\n"
              << "  " << program_name << " <file_path>\n"
              << "  " << program_name << " <file_path> <expected_sha256_hex>\n";
}

} // namespace

int main(int argc, char* argv[]) {
    try {
        if (argc != 2 && argc != 3) {
            print_usage(argv[0]);
            return 1;
        }

        const std::string file_path = argv[1];
        const std::string actual_hash = sha256::calculate_sha256_file(file_path);

        if (argc == 2) {
            std::cout << actual_hash << "\n";
            return 0;
        }

        const std::string expected_hash = to_lower_hex(argv[2]);

        // CHỖ SỬA QUAN TRỌNG: In ra chính xác [PASS] hoặc [FAIL] theo yêu cầu nghiêm ngặt của CI
        if (actual_hash == expected_hash) {
            std::cout << "[PASS]\n";
            return 0;
        }

        std::cout << "[FAIL]\n";
        return 1;
    } catch (const std::exception& ex) {
        // Nếu có lỗi (ví dụ file không tồn tại), hệ thống cũng tính là FAIL toàn vẹn
        std::cout << "[FAIL]\n";
        return 1;
    }
}