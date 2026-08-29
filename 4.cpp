#include <iostream>
#include <filesystem>
#include <chrono>

namespace fs = std::filesystem;

void move_old_txt_files(const fs::path& src_dir, const fs::path& dest_dir) {
    fs::create_directories(dest_dir);
    auto now = fs::file_time_type::clock::now();
    auto one_year = std::chrono::hours(24 * 365);

    for (const auto& entry : fs::recursive_directory_iterator(src_dir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            auto lwt = fs::last_write_time(entry.path());
            if (now - lwt > one_year) {
                fs::path target = dest_dir / entry.path().filename();
                fs::rename(entry.path(), target);
                std::cout << "Moved: " << entry.path().filename() << " to " << dest_dir << '\n';
            }
        }
    }
}