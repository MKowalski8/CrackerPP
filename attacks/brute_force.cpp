#include "brute_force.h"
#include "../hashes/hash_util.h"
#include <cmath>
#include <iostream>
#include <atomic>
#include <ncurses.h>
#include <thread>
#include <ncurses.h>

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

void BruteForce::startAttack(int threadNum) {
    std::vector<std::thread> threads;

    for (int i = 0; i < threadNum; ++i) {
        threads.emplace_back(&BruteForce::threadWorker, this, i, threadNum);
    }

    for (auto &thread: threads) {
        thread.join();
    }
}


void BruteForce::threadWorker(int threadId, int threadCount) {
    for (auto &hashGroup: hashes) {
        for (auto hashFunc: hashGroup.getHashFunctions()) {
            std::string word;
            word = charset[threadId];

            for (int i = threadId; i < possibleWordNumber; i += threadCount) {
                std::string hashedWord = HashUtil::computeHash(word, hashFunc);

                {
                    std::lock_guard<std::mutex> lock(mtx);
                    if (hashGroup.containsHash(hashedWord)) {
                        this->addBrokenHash(BrokenHash(hashedWord, EVP_MD_name(hashFunc), word));
                        hashGroup.eraseHash(hashedWord);
                    }
                }

                if (!hashGroup.empty()) {
                    generateWord(word);
                } else {
                    break;
                }
            }

            if (hashGroup.empty()) break;
        }

    }
}

void BruteForce::generateWord(std::string &word) {
    for (int i = word.length() - 1; i >= 0; --i) {
        word[i] = nextCharMap[word[i]];
        if (word[i] != charset[0]) {
            return;
        }
    }
    word = charset[0] + word;
}


long long BruteForce::totalPossibleWords() {
    int charsetLength = (int) charset.length();
    long long total = 0;
    for (int length = 1; length <= maxLength; ++length) {
        total += std::pow(charsetLength, length);
    }
    return total;
}
