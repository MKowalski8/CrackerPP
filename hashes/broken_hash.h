//
// Created by maks on 6/2/24.
//

#ifndef CRACKERPP_BROKEN_HASH_H
#define CRACKERPP_BROKEN_HASH_H

#include <openssl/evp.h>
#include <string>
#include <sstream>

class BrokenHash {

private:
    const std::string hashType;
    const std::string hash;
    const std::string word;
public:
    BrokenHash(std::string hash, std::string hashType, std::string word);

    [[nodiscard]] const std::string &getHashType() const;

    [[nodiscard]] const std::string &getHash() const;

    [[nodiscard]] const std::string &getWord() const;


    [[nodiscard]] std::string toString() const;
};

#endif //CRACKERPP_BROKEN_HASH_H
