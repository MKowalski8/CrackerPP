//
// Created by maks on 6/8/24.
//

#include <iostream>
#include <sstream>
#include "grouped_hashes.h"
#include <fmt/color.h>

GroupedHashes::GroupedHashes(const std::unordered_set<std::string> &groupedHashes, const EVP_MD *hashFunc) : hashes(
        groupedHashes) {
    if (!groupedHashes.empty()) {
        auto firstHash = hashes.begin();
        if (hashFunc != nullptr) {
            this->hashFunctions.emplace_back(hashFunc);
        } else {
            this->hashFunctions = HashIdentifier::identifyHash(*firstHash);
        }
    }
}

std::string GroupedHashes::toString() const {
    std::stringstream ss;
    for (const auto &hash : hashes) {
        if (!hashFunctions.empty()) {
            ss << fmt::format(fg(fmt::color::beige), "{}", hash)  << " - " << fmt::format(fg(fmt::color::red), "{}", EVP_MD_name(hashFunctions[0])) << std::endl;
        } else {
            ss << fmt::format(fg(fmt::color::beige), "{}", hash) << std::endl;
        }
    }
    return ss.str();
}

std::string GroupedHashes::toStringVerbose() const {
    std::stringstream ss;
    for (const auto &hash : hashes) {
        ss << fmt::format(fg(fmt::color::beige), "{}", hash) << " - ";
        for (const auto &func : hashFunctions) {
            ss << fmt::format(fg(fmt::color::red), "{}", EVP_MD_name(func)) << ", ";
        }
        ss << std::endl;
    }
    return ss.str();
}