//
// Created by maks on 6/12/24.
//

#ifndef CRACKER_MODE_H
#define CRACKER_MODE_H


#include <cctype>
#include <string>
#include <algorithm>

enum class Mode {
    Dictionary,
    Brute,
    Identify,
    Unknown
};

Mode parseMode(const std::string& modeStr) {
    std::string mode = modeStr;
    std::transform(mode.begin(), mode.end(), mode.begin(), ::tolower);

    if (mode == "dictionary") {
        return Mode::Dictionary;
    } else if (mode == "brute") {
        return Mode::Brute;
    } else if (mode == "identify") {
        return Mode::Identify;
    } else {
        return Mode::Unknown;
    }
}


#endif //CRACKER_MODE_H
