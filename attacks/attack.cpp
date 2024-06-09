//
// Created by maks on 6/7/24.
//

#include <iostream>
#include <algorithm>
#include "attack.h"
#include "../file_loader.h"
#include "../hashes/hash_identifier.h"

Attack::Attack(const std::string &hashStr, DataSource dataSource, const std::string &hashType) {
    const EVP_MD *hashFunc = HashIdentifier::getEVP_MD(hashType);

    switch (dataSource) {
        case DataSource::FILE_PATHS:
        case DataSource::HASH_LIST: {
            if (!hashType.empty()) {
                auto set = FileLoader::load(hashStr);
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

