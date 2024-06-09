#include "dictionary.h"
#include <iostream>
#include <vector>
#include <functional>
#include "../file_loader.h"
#include "../hashes/hash_util.h"
#include "DataSource.h"


DictionaryAttack::DictionaryAttack(const std::string &hashStr, const std::string &wordStr, DataSource dataSource,
                                   const std::string &hashType)
        : Attack(hashStr, dataSource, hashType) {
    switch (dataSource) {
        case DataSource::WORD_LIST:
        case DataSource::FILE_PATHS: {
            this->wordlist = FileLoader::load(wordStr);
            break;
        }
        case DataSource::STRINGS:
        case DataSource::HASH_LIST: {
            this->wordlist.insert(wordStr);
            break;

        }
    }
}

void DictionaryAttack::startAttack() {
    for (auto &hashGroup: hashes) {
        if (!hashes.empty()) {
            for (auto hashFunc: hashGroup.getHashFunctions()) {
                for (const auto &word: wordlist) {
                    std::string hashedWord = HashUtil::computeHash(word, hashFunc);

                    if (hashGroup.contains(hashedWord)) {
                        this->addBrokenHash(BrokenHash(hashedWord, EVP_MD_name(hashFunc), word));
                        hashGroup.erase(hashedWord);
                    }
                }
            }
        }
    }
}



//void DictionaryAttack::applyRules(std::vector<std::string>& wordlist) {
//    std::vector<std::string> newWords;
//
//    for (const auto& word : wordlist) {
//        // Example rules: append numbers, substitute characters
//        for (int i = 0; i < 10; ++i) {
//            newWords.push_back(word + std::to_string(i));
//        }
//        // Add more rules as needed
//    }
//
//    wordlist.insert(wordlist.end(), newWords.begin(), newWords.end());
//}

