//
// Created by maks on 6/7/24.
//

#ifndef CRACKERPP_HASH_IDENTIFIER_H
#define CRACKERPP_HASH_IDENTIFIER_H

#include <openssl/evp.h>
#include <string>
#include <map>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "grouped_hashes.h"


class GroupedHashes;

class HashIdentifier {
private:
    static const std::unordered_map<size_t, std::vector<const EVP_MD *>> hashAlgorithms;
    static const std::map<std::string, const EVP_MD *> hashFunctionMap;
    std::vector<GroupedHashes> hashes;

public:
    explicit HashIdentifier(const std::string &filePath);

    static std::vector<const EVP_MD *> identifyHash(const std::string &hash);

    std::vector<GroupedHashes> getHashesWithFunction();

    static const EVP_MD *getEVP_MD(const std::string &hashFunctionName);

    std::string toString() const;

    std::string toStringVerbose() const;
};


#endif //CRACKERPP_HASH_IDENTIFIER_H
