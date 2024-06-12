#include "hash_identifier.h"
#include "../file_manager.h"
#include <openssl/evp.h>
#include <string>
#include <stdexcept>
#include <cstring>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <sstream>


HashIdentifier::HashIdentifier(const std::string &filePath) {
    std::unordered_set<std::string> toClassify = FileManager::loadList(filePath);

    std::unordered_map<size_t, std::unordered_set<std::string>> classified;

    for (const auto &str: toClassify) {
        classified[str.length()].insert(str);
    }

    for (const auto &pair: classified) {
        GroupedHashes groupedHashes = GroupedHashes(pair.second);
        hashes.emplace_back(groupedHashes);
    }

    std::sort(hashes.begin(), hashes.end(), [](const GroupedHashes &a, const GroupedHashes &b) {
        return a.getHashesNumber() < b.getHashesNumber();
    });
}


std::vector<const EVP_MD *> HashIdentifier::identifyHash(const std::string &hash) {
    std::vector<const EVP_MD *> result;

    size_t bytes = hash.length() / 2;
    auto it = hashAlgorithms.find(bytes);
    if (it != hashAlgorithms.end()) {
        return it->second;
    }
    throw std::invalid_argument("Hash not identified");
}

std::vector<GroupedHashes> HashIdentifier::getHashesWithFunction() {
    return hashes;
}

const EVP_MD *HashIdentifier::getEVP_MD(const std::string &hashFunctionName) {
    auto it = hashFunctionMap.find(hashFunctionName);
    if (it != hashFunctionMap.end()) {
        return it->second;
    } else {
        return nullptr;
    }
}

std::string HashIdentifier::toString() const {
    std::stringstream ss;
    for (const auto &groupedHashes : hashes) {
        ss << groupedHashes.toString();
    }
    return ss.str();
}

std::string HashIdentifier::toStringVerbose() const {
    std::stringstream ss;
    for (const auto &groupedHashes : hashes) {
        ss << groupedHashes.toStringVerbose();
    }
    return ss.str();
}


const std::unordered_map<size_t, std::vector<const EVP_MD *>> HashIdentifier::hashAlgorithms = {
        {16, {EVP_md5(),    EVP_shake128(), EVP_md4(),        EVP_mdc2()}},
        {20, {EVP_sha1(),   EVP_md5_sha1(), EVP_ripemd160()}},
        {28, {EVP_sha224(), EVP_sha3_224()}},
        {32, {EVP_sha256(), EVP_sha3_256(), EVP_sha512_256(), EVP_blake2s256(), EVP_sm3(), EVP_shake256()}},
        {48, {EVP_sha384(), EVP_sha3_384()}},
        {64, {EVP_sha512(), EVP_sha3_512(), EVP_blake2b512(), EVP_whirlpool()}}
};

const std::map<std::string, const EVP_MD *> HashIdentifier::hashFunctionMap = {
        {"md4",         EVP_md4()},
        {"md5",         EVP_md5()},
        {"md5-sha1",    EVP_md5_sha1()},
        {"blake2b-512", EVP_blake2b512()},
        {"blake2s-256", EVP_blake2s256()},
        {"sha-1",       EVP_sha1()},
        {"sha-224",     EVP_sha224()},
        {"sha-256",     EVP_sha256()},
        {"sha-384",     EVP_sha384()},
        {"sha-512",     EVP_sha512()},
        {"sha-512/224", EVP_sha512_224()},
        {"sha-512/256", EVP_sha512_256()},
        {"sha3-224",    EVP_sha3_224()},
        {"sha3-256",    EVP_sha3_256()},
        {"sha3-384",    EVP_sha3_384()},
        {"sha3-512",    EVP_sha3_512()},
        {"shake128",    EVP_shake128()},
        {"shake256",    EVP_shake256()},
        {"mdc2",        EVP_mdc2()},
        {"ripemd-160",  EVP_ripemd160()},
        {"whirlpool",   EVP_whirlpool()},
        {"sm3",         EVP_sm3()},
};

