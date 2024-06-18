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

#endif