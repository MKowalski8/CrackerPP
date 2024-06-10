//
// Created by maks on 6/7/24.
//

#include "brutte_force.h"
#include "../hashes/hash_util.h"
#include <cmath>
#include <iostream>

BruteForce::BruteForce(const std::string &filePath, DataSource dataSource, const std::string &hashType, int maxLength,
                       const std::string &givenCharset)
        : Attack(filePath, dataSource, hashType), maxLength(maxLength) {
    this->possibleWordNumber = totalPossibleWords();

    if (!givenCharset.empty()) this->charset = givenCharset;


//    Using a map for holding the charset would be more time efficient, while generating words dynamically
    for (size_t i = 0; i < charset.size(); ++i) {
        nextCharMap[charset[i]] = (i < charset.size() - 1) ? charset[i + 1] : charset[0];
    }
}

void BruteForce::startAttack() {
    int j = 0;

    for (auto &hashGroup: hashes) {
        for (auto hashFunc: hashGroup.getHashFunctions()) {
            std::string word;
            word = charset[0];

            for (int i = 0; i < possibleWordNumber; i++) {
                std::string hashedWord = HashUtil::computeHash(word, hashFunc);

//                std::cout << word << " ";
                if (hashGroup.containsHash(hashedWord)) {
                    this->addBrokenHash(BrokenHash(hashedWord, EVP_MD_name(hashFunc), word));
                    hashGroup.eraseHash(hashedWord);
                    if (hashGroup.empty()) break;
                }

                generateWord(word);
                j++;
            }
//            std::cout << EVP_MD_name(hashFunc) << std::endl;
            if (hashGroup.empty()) break;
        }
    }
//    std::cout << j << " " << possibleWordNumber << std::endl;
}

// It should be more time efficient
void BruteForce::generateWord(std::string &word) {
    for (int i = word.length() - 1; i >= 0; --i) {
        word[i] = nextCharMap[word[i]];
        if (word[i] != charset[0]) {
            return;
        }
    }
    word = charset[0] + word;
}

//void BruteForce::generateWord(std::string &word) {
//    for (int i = word.length() - 1; i >= 0; --i) {
//        auto pos = charset.find(word[i]);
//        if (pos != std::string::npos && pos < charset.length() - 1) {
//            word[i] = charset[pos + 1];
//            return;
//        } else {
//            word[i] = charset[0];
//        }
//    }
//    word = charset[0] + word;
//}



long long BruteForce::totalPossibleWords() {
    int charsetLength = (int) charset.length();
    long long total = 0;
    for (int length = 1; length <= maxLength; ++length) {
        total += std::pow(charsetLength, length);
    }
    return total;
}