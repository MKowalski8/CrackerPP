#include <fstream>
#include "file_manager.h"

std::unordered_set<std::string> FileManager::loadList(const std::string &filePath) {
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

void FileManager::saveOutput(const std::string &filePath, const std::string &output) {
    std::ofstream outputFile(filePath);
    if (outputFile.is_open()) {
        outputFile << output;
        std::cout << "Output saved to file: " << filePath << std::endl;
        outputFile.close();
    } else {
        std::cerr << "Error opening file for writing: " << filePath << std::endl;
    }
}
