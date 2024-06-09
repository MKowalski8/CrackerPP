#include <argparse/argparse.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <fmt/color.h>
#include "hashes/broken_hash.h"
#include "attacks/attack.h"
#include "attacks/dictionary.h"
#include "attacks/brutte_force.h"

int main(int argc, char **argv) {
    argparse::ArgumentParser program("CRACKER++");

    program.add_argument("mode")
            .help("Mode: dictionary, brute-force or hash-identifier");

    program.add_argument("--hash-file")
            .help("File containing hashes");

    program.add_argument("--hash-value")
            .help("Single hash value");

    program.add_argument("--wordlist")
            .help("Path to wordlist file");

    program.add_argument("--word")
            .help("Single word");

    program.add_argument("--charset")
            .help("Character set for brute-force");

    program.add_argument("--max_len")
            .help("Maximum length of passwords to try")
            .default_value(8)
            .scan<'i', int>();

    program.add_argument("--threads")
            .help("Number of threads to use")
            .default_value(1)
            .scan<'i', int>();

    program.add_argument("--hash-type")
            .help("Type of hash algorithm (e.g., sha256, md5, sha1)");

    program.add_argument("--verbose")
            .default_value(false)
            .implicit_value(true)
            .help("Write all of the possible hash algorithms while identifying");

    try {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error &err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }

    auto mode = program.get<std::string>("mode");
    std::transform(mode.begin(), mode.end(), mode.begin(), ::tolower);

    std::string charset = program.present("--charset") ? program.get<std::string>("--charset") : "";
    std::string word = program.present("--word") ? program.get<std::string>("--word") : "";
    std::string hashList = program.present("--hash-file") ? program.get<std::string>("--hash-file") : "";
    std::string hashValue = program.present("--hash-value") ? program.get<std::string>("--hash-value") : "";
    std::string wordList = program.present("--wordlist") ? program.get<std::string>("--wordlist") : "";
    bool verbose = program.get<bool>("--verbose");

    int maxLen = program.get<int>("max_len");
    int numThreads = program.get<int>("threads");
    std::string hashType = program.present("--hash-type") ? program.get<std::string>("--hash-type") : "";


    if (hashList.empty() && hashValue.empty()) {
        std::cerr << "Either --hash-file or --hash-value must be provided." << std::endl;
        return 1;
    }

    Attack *attack = nullptr;

    if (mode.find("dictionary") != std::string::npos) {

        try {
            if (!wordList.empty() && !hashList.empty()) {
                attack = new DictionaryAttack(hashList, wordList, DataSource::FILE_PATHS, hashType);
            } else if (!hashList.empty() && !word.empty()) {
                attack = new DictionaryAttack(hashList, word, DataSource::HASH_LIST, hashType);
            } else if (!wordList.empty() && !hashValue.empty()) {
                attack = new DictionaryAttack(wordList, hashValue, DataSource::WORD_LIST, hashType);
            } else if (!word.empty()) {
                attack = new DictionaryAttack(hashValue, word, DataSource::STRINGS, hashType);
            } else {
                std::cerr << "Either --wordlist or --word must be provided." << std::endl;
                return 1;
            }
        } catch (const std::exception &err) {
            std::cerr << err.what() << std::endl;
        }


    } else if (mode.find("brute-force") != std::string::npos) {
        try {
            if (!hashList.empty()) {
                attack = new BruteForce(hashList, DataSource::FILE_PATHS, hashType, maxLen);
            } else {
                attack = new BruteForce(hashValue, DataSource::FILE_PATHS, hashType, maxLen);
            }
        } catch (const std::exception &err) {
            std::cerr << err.what() << std::endl;
        }
    } else if (mode.find("hash-identifier") != std::string::npos) {
        try {
            std::stringstream ss;
            std::cout << fmt::format(fg(fmt::color::beige), "   HASH") << "    |    ";
            std::cout << fmt::format(fg(fmt::color::red), "PROBABLE HASH ALGORITHM") << std::endl << std::endl;

            if (!hashList.empty()) {
                HashIdentifier identifier = HashIdentifier(hashList);
                if (verbose) {
                    ss << identifier.toStringVerbose();
                } else {
                    ss << identifier.toString();
                }
            } else {
                std::unordered_set<std::string> set;
                set.insert(hashValue);
                GroupedHashes hash = GroupedHashes(set);

                if (verbose) {
                    ss << hash.toStringVerbose();
                } else {
                    ss << hash.toString();
                }
            }
            std::cout << ss.str();
        } catch (const std::exception &err) {
            std::cerr << err.what() << std::endl;
        }
    } else {
        std::cerr << "Provided mode does not exist" << std::endl;
        std::cerr << program;

        return 1;
    }


    if (attack != nullptr) {
        attack->startAttack();
        std::vector<BrokenHash> brokenHashes = attack->getBrokenHashes();

        if (!brokenHashes.empty()) {
            std::cout << fmt::format("{} | {} | {}", fmt::format(fg(fmt::color::beige), "HASH"),
                                     fmt::format(fg(fmt::color::green_yellow), "PLAIN TEXT"),
                                     fmt::format(fg(fmt::color::red), "HASH ALGORITHM")) << std::endl << std::endl;

            for (const auto &brokenHash: brokenHashes) {
                std::cout << brokenHash.toString();
            }
        } else {
            std::cout << fmt::format(fg(fmt::color::red), "NO HASH BROKEN");
        }
    }
    return 0;
}
