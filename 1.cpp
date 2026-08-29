#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

void rename_c_to_cpp(const fs::path& dir) {
    for (const auto& entry : fs::recursive_directory_iterator(dir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".c") {
            fs::path new_path = entry.path();
            new_path.replace_extension(".cpp");
            fs::rename(entry.path(), new_path);
            std::cout << "Renamed: " << entry.path().filename() << " -> " << new_path.filename() << '\n';
        }
    }
}