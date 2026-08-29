#include <iostream>
#include <filesystem>
#include <chrono>

namespace fs = std::filesystem;

void delete_txt_older_than(const fs::path& dir, fs::file_time_type cutoff_time) {
    for (const auto& entry : fs::recursive_directory_iterator(dir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            auto lwt = fs::last_write_time(entry.path());
            if (lwt < cutoff_time) {
                fs::remove(entry.path());
                std::cout << "Deleted old file: " << entry.path().filename() << '\n';
            }
        }
    }
}