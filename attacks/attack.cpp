#include <iostream>
#include <algorithm>
#include <curses.h>
#include <fmt/color.h>
#include "attack.h"
#include "../file_manager.h"
#include "../hashes/hash_identifier.h"

Attack::Attack(const std::string &hashStr, DataSource dataSource, const std::string &hashType) {
    const EVP_MD *hashFunc = HashIdentifier::getEVP_MD(hashType);

    switch (dataSource) {
        case DataSource::FILE_PATHS:
        case DataSource::HASH_LIST: {
            if (!hashType.empty()) {
                auto set = FileManager::loadList(hashStr);
                GroupedHashes groupedHashes = GroupedHashes(set, hashFunc);
                this->hashes.emplace_back(groupedHashes);
            } else {
                HashIdentifier hashIdentifier = HashIdentifier(hashStr);
                this->hashes = hashIdentifier.getHashesWithFunction();
            }

            break;
        }
        case DataSource::STRINGS:
        case DataSource::WORD_LIST: {
            std::unordered_set<std::string> set;
            set.insert(hashStr);
            GroupedHashes groupedHashes = GroupedHashes(set, hashFunc);
            this->hashes.emplace_back(groupedHashes);
            break;
        }
    }

}

void Attack::progressBar() {
    std::lock_guard<std::mutex> lock(mtx);
    float progress = static_cast<float>(progressCounter) / totalIterations;
    int barWidth = 70;

    int pos = static_cast<int>(barWidth * progress);
    std::string progressBarStr = fmt::format("\r[");
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) {
            progressBarStr += "=";
        } else if (i == pos) {
            progressBarStr += ">";
        } else {
            progressBarStr += " ";
        }
    }
    progressBarStr += fmt::format("] {:.1f}% ", progress * 100.0);

    if (progress < 0.25) {
        progressBarStr = fmt::format(fmt::fg(fmt::color::red) | fmt::emphasis::bold, "{}", progressBarStr);
    } else if (progress < 0.5) {
        progressBarStr = fmt::format(fmt::fg(fmt::color::yellow) | fmt::emphasis::bold, "{}", progressBarStr);
    } else if (progress < 0.75) {
        progressBarStr = fmt::format(fmt::fg(fmt::color::yellow_green) | fmt::emphasis::bold, "{}", progressBarStr);
    } else {
        progressBarStr = fmt::format(fmt::fg(fmt::color::light_blue) | fmt::emphasis::bold, "{}", progressBarStr);
    }

    fmt::print(progressBarStr);
    std::cout.flush();
}

const std::vector<BrokenHash> &Attack::getBrokenHashes() const {
    return brokenHashes;
}

void Attack::checkHash(GroupedHashes &hashGroup, const EVP_MD *hashFunc, const std::string &word,
                                 std::string &hashedWord) {
    std::lock_guard<std::mutex> lock(mtx);
    if (hashGroup.containsHash(hashedWord)) {
        addBrokenHash(BrokenHash(hashedWord, EVP_MD_name(hashFunc), word));
        hashGroup.eraseHash(hashedWord);
    }
    ++progressCounter;
}

