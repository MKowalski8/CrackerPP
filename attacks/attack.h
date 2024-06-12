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
#ifndef CRACKERPP_ATTACK_H
#define CRACKERPP_ATTACK_H


class Attack {
private:
    std::vector<BrokenHash> brokenHashes;

protected:
    std::vector<GroupedHashes> hashes;

public:
    explicit Attack(const std::string& filePath, DataSource dataSource, const std::string& hashType);

    [[nodiscard]] const std::vector<BrokenHash> &getBrokenHashes() const;

    virtual void startAttack() {};

    void addBrokenHash(const BrokenHash& brokenHash){
        brokenHashes.emplace_back(brokenHash);
     }
};

#endif //CRACKERPP_ATTACK_H
