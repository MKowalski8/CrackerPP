#include "dictionary.h"
#include <iostream>
#include <vector>
#include <functional>
#include <thread>
#include "../file_manager.h"
#include "../hashes/hash_util.h"
#include "data_source.h"


DictionaryAttack::DictionaryAttack(const std::string &hashStr, const std::string &wordStr, DataSource dataSource,
                                   const std::string &hashType)
        : Attack(hashStr, dataSource, hashType) {
    switch (dataSource) {
        case DataSource::WORD_LIST:
        case DataSource::FILE_PATHS: {
            this->wordlist = FileManager::loadList(wordStr);
            break;
        }
        case DataSource::STRINGS:
        case DataSource::HASH_LIST: {
            this->wordlist.insert(wordStr);
            break;

        }
    }
}

void DictionaryAttack::startAttack(int threadNum) {
    std::vector<std::thread> threads;
    totalIterations = operationsCounter();

    threadNum = wordlist.size() - 1 < threadNum ? wordlist.size() - 1 : threadNum;

    for (int i = 0; i < threadNum; ++i) {
        threads.emplace_back(&DictionaryAttack::threadWorker, this, i, threadNum);
    }

    for (auto &thread: threads) {
        thread.join();
    }
}


void DictionaryAttack::threadWorker(int threadId, int threadNum) {
    for (auto &hashGroup: hashes) {
        if (!hashGroup.empty()) {
            for (auto hashFunc: hashGroup.getHashFunctions()) {
                auto it = wordlist.begin();
                std::advance(it, threadId);


                while (it != wordlist.end()) {
                    const std::string &word = *it;
                    std::string hashedWord = HashUtil::computeHash(word, hashFunc);

                    checkHash(hashGroup, hashFunc, word, hashedWord);
                    for (int i = 0; i < threadNum; i++) {
                        if (it == wordlist.end()) break;
                        it++;
                    }
                    progressBar();
                    if (hashGroup.empty()) break;
                }
                if (hashGroup.empty()) break;
            }
        }
    }
}


size_t DictionaryAttack::operationsCounter() {
   int operations = 0;

    for (auto &hashGroup: hashes){
        operations += hashGroup.getHashFunctions().size() * wordlist.size();
    }

    return operations;
}
