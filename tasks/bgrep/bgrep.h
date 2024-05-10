#pragma once

#include <filesystem>
#include <optional>
#include <algorithm>
#include <iterator>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <regex>

enum class GrepAlgorithm {
    kRegular,
    kBoyerMoore
};

struct GrepOptions {
    GrepAlgorithm algo;
    size_t max_matches;
    size_t offset = 0;

    GrepOptions() {
        algo = GrepAlgorithm::kRegular;
        max_matches = -1;
    }
};


std::vector<std::optional<uint8_t>> parse_byte_pattern(const std::string& pattern) {
    std::regex regExpr("^((?:[0-9a-fA-F]{2}|\\?\\?)\\s)*([0-9a-fA-F]{2}|\\?\\?)$");

    if (!std::regex_match(pattern, regExpr))
        throw "invalid patern";

    std::vector<std::optional<uint8_t>> byte_pattern;
    size_t len = pattern.length();
    for (size_t i = 0; i < len - 1; i += 3) {
        if (pattern[i] == '?' && pattern[i + 1] == '?') {
            byte_pattern.emplace_back(std::nullopt);
        } else {
            try {
                uint8_t byte = std::stoi(pattern.substr(i, 2), nullptr, 16);
                byte_pattern.emplace_back(byte);
            } catch (...) {
                throw "invalid patern";
            }
        }
        if (i + 2 < len && pattern[i + 2] != ' '){
            throw "invalid patern space";
        }
    }
    return byte_pattern;
}

bool is_simple_pattern(const std::string& pattern) {
    return pattern.find("??") == std::string::npos;
}

template<class Visitor>
void grep_file(const std::string& file_path, const std::vector<std::optional<uint8_t>>& pattern, bool isSimplePattern, Visitor visitor, const GrepOptions& options) {
    int fd = open(file_path.c_str(), O_RDONLY);
    if (fd == -1) {
        visitor.OnError("Failed to open file: " + file_path);
        return;
    }

    struct stat st;
    if (fstat(fd, &st) == -1) {
        close(fd);
        visitor.OnError("Failed to get file size: " + file_path);
        return;
    }

    off_t file_size = st.st_size;
    long page_size = sysconf(_SC_PAGE_SIZE);
    off_t offset = std::min(options.offset, static_cast<size_t>(file_size));
    offset = (offset / page_size) * page_size;
    off_t remaining_size = file_size - offset;

    if (remaining_size <= 0) {
        close(fd);
        return;
    }

    unsigned char* data = static_cast<unsigned char*>(mmap(NULL, remaining_size, PROT_READ, MAP_PRIVATE, fd, offset));
    if (data == MAP_FAILED) {
        close(fd);
        visitor.OnError("Failed to mmap file: " + file_path);
        return;
    }

    const unsigned char* match_start = data + (offset == 0 ? offset : options.offset % offset);
    const unsigned char* end = data + remaining_size;
    size_t matches = 0;

    if (isSimplePattern && options.algo == GrepAlgorithm::kBoyerMoore) {
        std::vector<unsigned char> currPattern;
        currPattern.reserve(pattern.size());
        for (auto elem : pattern){
            currPattern.push_back(elem.value());
        }

        auto searcher = std::boyer_moore_searcher(currPattern.begin(), currPattern.end());

        while (true) {
            const unsigned char* found_match = std::search(match_start, end, searcher);
            if (found_match == end) {
                break;
            }
            visitor.OnMatch(file_path, found_match - data + options.offset, found_match, currPattern.size());
            ++matches;
            match_start = found_match + 1;
            if (matches >= options.max_matches) {
                break;
            }
        }
    } else if (isSimplePattern){ // Regular algorithm
        std::vector<unsigned char> pattern_bytes;
        pattern_bytes.reserve(pattern.size());
        for (const auto& opt_byte : pattern) {
            pattern_bytes.push_back(opt_byte.value());
        }
        const auto pattern_size = pattern.size();
        const auto data_end = end - pattern_size;
        const auto pattern_data = pattern_bytes.data();
        while (match_start <= data_end) {
            if (std::memcmp(match_start, pattern_data, pattern_size) == 0) {
                visitor.OnMatch(file_path, match_start - data +(offset == 0 ? options.offset : offset), match_start, pattern_size);
                ++matches;
                match_start += pattern_size;
            } else {
                ++match_start;
            }
            if (matches >= options.max_matches) {
                break;
            }
        }
    }
    else{
        const auto pattern_size = pattern.size();
        while (true) {
            const unsigned char* found_match = std::search(match_start, end, pattern.begin(), pattern.end(), [] (unsigned char left, std::optional<uint8_t> right){
                if (!right)
                    return true;
                return left == right.value();
            });
            if (found_match == end) {
                break;
            }
            visitor.OnMatch(file_path, found_match - data + +(offset == 0 ? options.offset : offset), found_match, pattern.size());
            ++matches;
            match_start = found_match + pattern_size;
            if (matches >= options.max_matches) {
                break;
            }
        }
    }

    munmap(data, remaining_size);
    close(fd);
}

template<class Visitor>
void grep_directory(const std::string& directory_path, const std::vector<std::optional<uint8_t>>& pattern, bool isSimplePattern, Visitor visitor, const GrepOptions& options) {
    for (const auto& entry : std::filesystem::recursive_directory_iterator(directory_path)) {
        if (!std::filesystem::is_regular_file(entry)) {
            continue;
        }
        grep_file(entry.path().string(), pattern, isSimplePattern, visitor, options);
    }
}

template <class Visitor>
inline void Grep(const std::string& path, const std::vector<std::optional<uint8_t>>& pattern, Visitor visitor,
                 const GrepOptions& options){
    if (pattern.empty()) {
        visitor.OnError("Invalid pattern");
        return;
    }
    bool isSimple = (std::find(pattern.begin(), pattern.end(), std::nullopt) == pattern.end());
    if (std::filesystem::is_directory(path)) {
        grep_directory(path, pattern, isSimple, visitor, options);
    }
    else{
        grep_file(path, pattern, isSimple, visitor, options);
    }
}

template <class Visitor>
inline void Grep(const std::string& path, const std::string& pattern, Visitor visitor,
                 const GrepOptions& options){
    std::vector<std::optional<uint8_t>> byte_pattern = parse_byte_pattern(pattern);
    if (byte_pattern.empty()) {
        visitor.OnError("Invalid pattern");
        return;
    }
    bool isSimple = (std::find(byte_pattern.begin(), byte_pattern.end(), std::nullopt) == byte_pattern.end());
    if (std::filesystem::is_directory(path)) {
        grep_directory(path, byte_pattern, isSimple, visitor, options);
    }
    else{
        grep_file(path, byte_pattern, isSimple, visitor, options);
    }
}