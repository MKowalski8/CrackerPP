#include "hash_util.h"
#include <iomanip>
#include <sstream>
#include <openssl/evp.h>

std::string HashUtil::computeHash(const std::string &input, const EVP_MD *hashType) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int lengthOfHash = 0;

    EVP_MD_CTX *context = EVP_MD_CTX_new();

    if (context != nullptr) {
        if (EVP_DigestInit_ex(context, hashType, nullptr)) {
            if (EVP_DigestUpdate(context, input.c_str(), input.length())) {
                if (EVP_DigestFinal_ex(context, hash, &lengthOfHash)) {
                    std::ostringstream oss;
                    for (unsigned int i = 0; i < lengthOfHash; ++i) {
                        oss << std::hex << std::setw(2) << std::setfill('0') << (int) hash[i];
                    }
                    return oss.str();
                }
            }
        }
        EVP_MD_CTX_free(context);
    }

    return "";
}

