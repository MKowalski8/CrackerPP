#ifndef CRACKERPP_DICTIONARY_H
#define CRACKERPP_DICTIONARY_H


#include <string>
#include <vector>
#include <functional>
#include "attack.h"

class DictionaryAttack : public Attack {
    std::unordered_set<std::string> wordlist;
public:
    DictionaryAttack(const std::string &hashes, const std::string &words, DataSource dataSource,
                     const std::string &hashType);

    void startAttack(int threadNum) override;
private:
    void threadWorker(int threadId, int threadNum) override;

};


#endif //CRACKERPP_DICTIONARY_H
