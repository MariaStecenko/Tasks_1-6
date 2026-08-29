#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>

namespace fs = std::filesystem;

// 1. 
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

// 2. 
void convert_comments_in_file(const fs::path& file_path) {
    std::ifstream in(file_path);
    if (!in.is_open()) return;

    std::stringstream buffer;
    std::string line;

    while (std::getline(in, line)) {
        size_t pos = line.find("//");
        if (pos != std::string::npos) {
            std::string code_part = line.substr(0, pos);
            std::string comment_part = line.substr(pos + 2);
            line = code_part + "/*" + comment_part + " */";
        }
        buffer << line << '\n';
    }
    in.close();

    std::ofstream out(file_path, std::ios::trunc);
    out << buffer.rdbuf();
}

void process_all_cpp_comments(const fs::path& dir) {
    for (const auto& entry : fs::recursive_directory_iterator(dir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".cpp") {
            convert_comments_in_file(entry.path());
            std::cout << "Updated comments in: " << entry.path().filename() << '\n';
        }
    }
}

// 3. 
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

// 4. 
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

// 5. 
void delete_small_word_files(const fs::path& dir) {
    const uintmax_t limit_bytes = 100 * 1024;

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

// 6. 
void calculate_avg_txt_size() {
    std::cout << "Введіть шлях до директорії: ";
    std::string input_path;
    std::getline(std::cin, input_path);

    fs::path dir(input_path);

    if (!fs::exists(dir) || !fs::is_directory(dir)) {
        std::cerr << "Помилка: директорію не знайдено.\n";
        return;
    }

    uintmax_t total_size = 0;
    size_t count = 0;

    for (const auto& entry : fs::directory_iterator(dir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            total_size += fs::file_size(entry.path());
            count++;
        }
    }

    if (count == 0) {
        std::cout << "У директорії немає файлів .txt\n";
    } else {
        double avg_size = static_cast<double>(total_size) / count;
        std::cout << "Знайдено .txt файлів: " << count << '\n';
        std::cout << "Середній розмір: " << avg_size << " байт (" << (avg_size / 1024.0) << " КБ)\n";
    }
}

// головна функція main
int main() {
    // вказую папку для тестів 
    fs::path test_dir = "./test_folder";

    std::cout << "=== 1. Перейменування .c у .cpp ===\n";
    if (fs::exists(test_dir)) rename_c_to_cpp(test_dir);

    std::cout << "\n=== 2. Оновлення коментарів у .cpp ===\n";
    if (fs::exists(test_dir)) process_all_cpp_comments(test_dir);

    std::cout << "\n=== 6. Підрахунок середнього розміру ===\n";
    calculate_avg_txt_size();

    return 0;
}