//
// Created by maks on 6/7/24.
//

#ifndef CRACKERPP_FILE_LOADER_H
#define CRACKERPP_FILE_LOADER_H


#include <unordered_set>
#include <string>

class FileLoader {
public:
    static std::unordered_set<std::string> load(const std::string &filePath);
};


#endif //CRACKERPP_FILE_LOADER_H
