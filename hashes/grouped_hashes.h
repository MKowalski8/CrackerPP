//
// Created by maks on 6/8/24.
//

#ifndef CRACKERPP_GROUPED_HASHES_H
#define CRACKERPP_GROUPED_HASHES_H


#include <unordered_set>
#include <string>
#include <openssl/types.h>
#include <vector>
#include "hash_identifier.h"

class GroupedHashes {
private:
    std::unordered_set<std::string> hashes;
    std::vector<const EVP_MD *> hashFunctions;


public:
    explicit GroupedHashes(const std::unordered_set<std::string> &groupedHashes, const EVP_MD *hashFunc = nullptr);

    const std::vector<const EVP_MD *> &getHashFunctions() const {
        return hashFunctions;
    }

    size_t getHashesNumber() const { return hashes.size(); }

    void erase(std::string &hash) {
        hashes.erase(hash);
    }

    bool contains(std::string &hash) {
        if (hashes.find(hash) != hashes.end()) {
            return true;
        }
        return false;
    }

    std::string toString() const;
    std::string toStringVerbose() const;
};


#endif //CRACKERPP_GROUPED_HASHES_H
