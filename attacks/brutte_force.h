//
// Created by maks on 6/7/24.
//

#ifndef CRACKERPP_BRUTTE_FORCE_H
#define CRACKERPP_BRUTTE_FORCE_H


#include <string>
#include "DataSource.h"
#include "attack.h"

class BruteForce : public Attack {
private:
    int maxLength;
    std::string charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
                          " !\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
    std::unordered_map<char, char> nextCharMap;

    long long possibleWordNumber;
public:
    BruteForce(const std::string &filePath, DataSource dataSource, const std::string &hashType, int maxLength,
               const std::string &givenCharset);

    void startAttack() override;

private:

    long long int totalPossibleWords();

    void generateWord(std::string &word);
};


#endif //CRACKERPP_BRUTTE_FORCE_H
