#ifndef CRACKERPP_HASH_UTIL_H
#define CRACKERPP_HASH_UTIL_H

#include <string>
#include <openssl/evp.h>

class HashUtil {
public:
    static std::string computeHash(const std::string &input, const EVP_MD *hashType);
};

#endif //CRACKERPP_HASH_UTIL_H return const_cast<EVP_MD *>(EVP_sha256());
