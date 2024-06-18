#ifndef JUTCHSON_FILESYSTEM_HPP_
#define JUTCHSON_FILESYSTEM_HPP_

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <iterator>

inline std::string readWholeFile(const std::filesystem::path& path) {
    std::ifstream file{path};
    return {std::istreambuf_iterator{file}, std::istreambuf_iterator<char>{}};
}

inline std::vector<std::filesystem::path> directoryElements(const std::filesystem::path& path) {
    std::vector<std::filesystem::path> res;
    for (std::filesystem::directory_iterator iter{path}; iter != std::filesystem::directory_iterator{}; ++iter) {
        if (iter->is_directory() || iter->path().extension() == ".juon") {
            res.push_back(iter->path());
        }
    }
    return res;
}

#endif