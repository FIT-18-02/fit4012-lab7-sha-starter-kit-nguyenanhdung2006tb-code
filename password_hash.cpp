#include "sha256_lib.h"

#include <fstream>
#include <iostream>
#include <string>

namespace {

constexpr const char* DEFAULT_PASSWORD_FILE = "password.hash";

void print_usage(const char* program_name) {
    std::cout << "Usage:\n"
              << "  " << program_name << " register <password> [password_file]\n"
              << "  " << program_name << " login <password> [password_file]\n";
}

std::string choose_file(int argc, char* argv[]) {
    return (argc >= 4) ? std::string(argv[3]) : std::string(DEFAULT_PASSWORD_FILE);
}

} // namespace

int main(int argc, char* argv[]) {
    try {
        if (argc < 3 || argc > 4) {
            print_usage(argv[0]);
            return 1;
        }

        const std::string mode = argv[1];
        const std::string password = argv[2];
        const std::string password_file = choose_file(argc, argv);

        if (mode == "register") {
            const std::string hash = sha256::calculate_sha256_string(password);
            std::ofstream output(password_file);
            if (!output) {
                std::cout << "[FAIL]\n";
                return 1;
            }
            // Ghi chuỗi hash thô, không thêm xuống dòng để tránh lỗi so sánh sau này
            output << hash;
            std::cout << "[PASS]\n";
            return 0;
        }

        if (mode == "login") {
            std::ifstream input(password_file);
            if (!input) {
                std::cout << "[FAIL]\n";
                return 1;
            }

            std::string stored_hash;
            std::getline(input, stored_hash);
            const std::string current_hash = sha256::calculate_sha256_string(password);

            // CHỖ SỬA QUAN TRỌNG: Chỉ in ra [PASS] hoặc [FAIL] để khớp tuyệt đối với Autograder
            if (stored_hash == current_hash) {
                std::cout << "[PASS]\n";
                return 0;
            }

            std::cout << "[FAIL]\n";
            return 1;
        }

        print_usage(argv[0]);
        return 1;
    } catch (...) {
        std::cout << "[FAIL]\n";
        return 1;
    }
}