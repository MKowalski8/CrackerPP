#include <tr1/unordered_map>
#include <string>
#include <openssl/crypto.h>
#include <unordered_set>
#include <vector>
#include <unordered_map>
#include <map>
#include "../hashes/broken_hash.h"
#include "../hashes/grouped_hashes.h"
#include "data_source.h"
#include <atomic>
#include <mutex>

#ifndef CRACKERPP_ATTACK_H
#define CRACKERPP_ATTACK_H


class Attack {
private:
    std::vector<BrokenHash> brokenHashes;

protected:
    std::vector<GroupedHashes> hashes;
    std::mutex mtx;
    size_t progressCounter = 0;
    size_t totalIterations = 0;
public:
    explicit Attack(const std::string& filePath, DataSource dataSource, const std::string& hashType);

    [[nodiscard]] const std::vector<BrokenHash> &getBrokenHashes() const;

    virtual void startAttack(int threadNum) {};

    void addBrokenHash(const BrokenHash& brokenHash){
        brokenHashes.emplace_back(brokenHash);
     }

private:
    virtual void threadWorker(int threadId, int threadCount) {};
    virtual size_t operationsCounter() {return 0;};

protected:
    void progressBar();
    void checkHash(GroupedHashes &hashGroup, const EVP_MD *hashFunc, const std::string &word, std::string &hashedWord);
};

#endif //CRACKERPP_ATTACK_H
