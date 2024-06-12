#ifndef CRACKERPP_FILE_LOADER_H
#define CRACKERPP_FILE_LOADER_H


#include <unordered_set>
#include <string>
#include <iostream>

class FileManager {
public:
    static std::unordered_set<std::string> loadList(const std::string &filePath);
    static void saveOutput(const std::string& filePath, const std::string& output);
};


#endif //CRACKERPP_FILE_LOADER_H
