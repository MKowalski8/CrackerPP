//
// Created by maks on 6/7/24.
//

#include "brutte_force.h"
#include "../hashes/hash_util.h"
#include <cmath>
#include <iostream>

BruteForce::BruteForce(const std::string& filePath, DataSource dataSource, const std::string& hashType, int maxLength)
        : Attack(filePath, dataSource, hashType), maxLength(maxLength) {
    this->possibleWordNumber = totalPossibleWords();
}

void BruteForce::startAttack() {
    for (auto &hashGroup: hashes) {
        if (!hashes.empty()) {
            for (auto hashFunc: hashGroup.getHashFunctions()) {
                std::string word;

                for(int i = 0; i < totalPossibleWords(); i++) {
                    std::string hashedWord = HashUtil::computeHash(word, hashFunc);

                    if (hashGroup.contains(hashedWord)) {
                        this->addBrokenHash(BrokenHash(hashedWord, EVP_MD_name(hashFunc), word));
                        hashGroup.erase(hashedWord);
                    }

                    generateWord(word);
                    std::cout << "word" << std::endl;
                }
            }
        }
    }
}

void BruteForce::generateWord(std::string &word) {
    if (word.length() >= maxLength) {
        word = charset[0];
        return;
    }

    for (int i = word.length() - 1; i >= 0; --i) {
        auto pos = charset.find(word[i]);
        if (pos != std::string::npos && pos < charset.length() - 1) {
            word[i] = charset[pos + 1];
            return;
        } else {
            word[i] = charset[0];
        }
    }
    word = charset[0] + word;
}

long long  BruteForce::totalPossibleWords() {
    int charsetLength = (int) charset.length();
    long long total = 0;
    for (int length = 1; length <= maxLength; ++length) {
        total += std::pow(charsetLength, length);
    }
    return total;
}