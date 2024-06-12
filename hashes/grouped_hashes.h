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

    void eraseHash(std::string &hash) {
        hashes.erase(hash);
    }

    bool empty() {
        return hashes.empty();
    }

    bool containsHash(std::string &hash) {
        if (hashes.find(hash) != hashes.end()) {
            return true;
        }
        return false;
    }

    std::string toString() const;
    std::string toStringVerbose() const;

    std::string forFile() const;
    std::string forFileVerbose() const;
};


#endif //CRACKERPP_GROUPED_HASHES_H
