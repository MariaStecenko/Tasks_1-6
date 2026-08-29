#include <iostream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

void calculate_avg_txt_size() {
    std::cout << "Enter directory path: ";
    std::string input_path;
    std::getline(std::cin, input_path);

    // працює з будь-якими слешами та українськими літерами
    fs::path dir(input_path);

    if (!fs::exists(dir) || !fs::is_directory(dir)) {
        std::cerr << "Invalid directory path.\n";
        return;
    }

    uintmax_t total_size = 0;
    size_t count = 0;

    for (const auto& entry : fs::directory_iterator(dir)) { // перевіряє лише поточну директорію
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            total_size += fs::file_size(entry.path());
            count++;
        }
    }

    if (count == 0) {
        std::cout << "No text files found in the directory.\n";
    } else {
        double avg_size = static_cast<double>(total_size) / count;
        std::cout << "Total .txt files: " << count << '\n';
        std::cout << "Average size: " << avg_size << " bytes (" << (avg_size / 1024.0) << " KB)\n";
    }
}