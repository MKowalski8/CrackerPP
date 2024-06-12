#include "broken_hash.h"

#include <utility>
#include <fmt/color.h>
#include "hash_util.h"


BrokenHash::BrokenHash(std::string hash, std::string hashType, std::string word) : hash(std::move(hash)),
                                                                                   hashType(std::move(hashType)),
                                                                                   word(std::move(word)) {}

const std::string &BrokenHash::getHashType() const {
    return hashType;
}

const std::string &BrokenHash::getHash() const {
    return hash;
}

const std::string &BrokenHash::getWord() const {
    return word;
}

std::string BrokenHash::toString() const {
    return fmt::format("{} | {} | {}", fmt::format(fg(fmt::color::beige), "{}", hash),
                       fmt::format(fg(fmt::color::green_yellow), "{}", word),
                       fmt::format(fg(fmt::color::red), "{}\n", hashType));
}

std::string BrokenHash::forFile() const {
    return fmt::format("{} | {} | {}", hash, word, hashType);
}
