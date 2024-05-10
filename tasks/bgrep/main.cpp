#include <iostream>

#include "bgrep.h"
#include "commons.h"

#include <gflags/gflags.h>

DEFINE_string(matches, "", "Maximum number of matches (unbounded by default)");
DEFINE_int32(offset, 0, "Offset (0 by default)");
DEFINE_string(algo, "brute", "Algorithm to use (brute or boyer-moore, brute by default)");

#ifndef TASK_DIR
#define TASK_DIR "./"
#endif

inline std::string ConstructBasePath() {
    std::string result(TASK_DIR);
    return result;
}

const std::string kBasePath = ConstructBasePath();

struct Match {
    std::string path;
    std::vector<uint8_t> match;
    size_t offset;

    bool operator==(const Match& rhs) const {
        return path == rhs.path && match == rhs.match && offset == rhs.offset;
    }
};

std::ostream& operator<<(std::ostream& out, const Match& match) {
    out << "Path " << match.path << ", offset " << match.offset << ", data";
    for (auto x : match.match) {
        out << " " << static_cast<int>(x);
    }
    return out;
}

class CollectMatches : public BaseVisitor {
public:
    explicit CollectMatches(std::vector<Match>& matches) : matches_(matches) {}

    void OnMatch(const std::string& path, size_t offset, const void *data, size_t len) {
        auto ptr = reinterpret_cast<const uint8_t*>(data);
        std::vector<uint8_t> match(ptr, ptr + len);
        matches_.push_back(Match{path, std::move(match), offset});
    }

    void OnError(const std::string& what) {
        std::cerr << "Fail: " << what << "\n";
    }
private:
    std::vector<Match>& matches_;
};

void printHex(const std::vector<unsigned char>& data) {
    std::cout << std::hex << std::setfill('0');
    for (const auto& byte : data) {
        std::cout << std::setw(2) << static_cast<int>(byte);
        if (std::addressof(byte) != std::addressof(data.back()))
            std::cout << " ";
    }
    std::cout << std::dec;
}

int main(int argc, char* argv[]){
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    std::string pattern = argv[1];

    std::vector<std::string> files;
    for (int i = 2; i < argc; ++i) {
        files.emplace_back(argv[i]);
    }

    int maxMatches = FLAGS_matches.empty() ? -1 : std::stoi(FLAGS_matches);

    std::string algorithm = FLAGS_algo;

    GrepOptions options;
    options.algo = algorithm == "boyer-moore" ? GrepAlgorithm::kBoyerMoore : GrepAlgorithm::kRegular;
    options.max_matches = maxMatches;
    options.offset = FLAGS_offset;

    std::vector<Match> matches;
    CollectMatches visitor(matches);

    std::unordered_map<std::string, std::string> pathToOrig;

    // Perform the search
    for (const std::string& fileOrDir : files) {
        Grep(std::string("../bgrep/") + fileOrDir, pattern, visitor, options);
        pathToOrig[std::string("../bgrep/") + fileOrDir] = fileOrDir;
    }

    for(auto &match : matches){
        std::cout << pathToOrig[match.path] << ":" << match.offset << ":";
        printHex(match.match);
        std::cout << '\n';
    }
}