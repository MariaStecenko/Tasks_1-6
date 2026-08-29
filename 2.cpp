#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

void convert_comments_in_file(const fs::path& file_path) {
    std::ifstream in(file_path);
    if (!in.is_open()) return;

    std::stringstream buffer;
    std::string line;

    while (std::getline(in, line)) {
        size_t pos = line.find("//");
        if (pos != std::string::npos) {
            // міняю // коментар на /* ... */
            std::string code_part = line.substr(0, pos);
            std::string comment_part = line.substr(pos + 2);
            line = code_part + "/*" + comment_part + " */";
        }
        buffer << line << '\n';
    }
    in.close();

    // очищаю старий вміст і записую змінені рядки
    std::ofstream out(file_path, std::ios::trunc);
    out << buffer.rdbuf();
}

void process_all_cpp_comments(const fs::path& dir) {
    for (const auto& entry : fs::recursive_directory_iterator(dir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".cpp") {
            convert_comments_in_file(entry.path());
        }
    }
}