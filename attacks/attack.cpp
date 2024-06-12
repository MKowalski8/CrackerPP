#include <iostream>
#include <algorithm>
#include <curses.h>
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


const std::vector<BrokenHash> &Attack::getBrokenHashes() const {
    return brokenHashes;
}

