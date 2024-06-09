//
// Created by maks on 6/7/24.
//

#include <fstream>
#include "file_loader.h"

std::unordered_set<std::string> FileLoader::load(const std::string &filePath) {
    std::ifstream file(filePath);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filePath);
    }

    std::unordered_set<std::string> set;
    std::string string;
    while (file >> string) {
        set.insert(string);
    }

    file.close();
    return set;
}
