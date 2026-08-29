#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

void delete_small_word_files(const fs::path& dir) {
    const uintmax_t limit_bytes = 100 * 1024; // 100 КБ

    for (const auto& entry : fs::recursive_directory_iterator(dir)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            if (ext == ".doc" || ext == ".docx") {
                if (fs::file_size(entry.path()) < limit_bytes) {
                    fs::remove(entry.path());
                    std::cout << "Deleted small doc: " << entry.path().filename() << '\n';
                }
            }
        }
    }
}