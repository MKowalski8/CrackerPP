#include <argparse/argparse.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <fmt/color.h>
#include "hashes/broken_hash.h"
#include "attacks/attack.h"
#include "attacks/dictionary.h"
#include "attacks/brute_force.h"
#include "file_manager.h"
#include "mode.h"

int main(int argc, char **argv) {
    argparse::ArgumentParser program("CRACKER++");

    program.add_argument("mode")
            .help("Mode: dictionary, brute or identify");

    program.add_argument("--hash-file", "-hl")
            .help("File containing hashes");

    program.add_argument("--hash-value")
            .help("Single hash value");

    program.add_argument("--wordlist", "-w")
            .help("Path to wordlist file");

    program.add_argument("--word", "-sw")
            .help("Single word");

    program.add_argument("--charset")
            .help("Character set for brute-force");

    program.add_argument("--max-len", "-l")
            .help("Maximum length of passwords to try")
            .default_value(8)
            .scan<'i', int>();

    program.add_argument("--threads", "-t")
            .help("Number of threads to use")
            .default_value(1)
            .scan<'i', int>();

    program.add_argument("--hash-type", "-ht")
            .help("Type of hash algorithm (e.g., sha256, md5, sha1)");

    program.add_argument("--verbose", "-vb")
            .default_value(false)
            .implicit_value(true)
            .help("Write all of the possible hash algorithms while identifying");
    program.add_argument("--to-file", "-o")
            .help("Save output to file");

    try {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error &err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }

    std::string charset = program.present("--charset") ? program.get<std::string>("--charset") : "";
    std::string word = program.present("--word") ? program.get<std::string>("--word") : "";
    std::string hashList = program.present("--hash-file") ? program.get<std::string>("--hash-file") : "";
    std::string hashValue = program.present("--hash-value") ? program.get<std::string>("--hash-value") : "";
    std::string wordList = program.present("--wordlist") ? program.get<std::string>("--wordlist") : "";
    bool verbose = program.get<bool>("--verbose");

    int maxLen = program.get<int>("max-len");
    int numThreads = program.get<int>("threads");
    std::string hashType = program.present("--hash-type") ? program.get<std::string>("--hash-type") : "";
    std::string toFile = program.present("--to-file") ? program.get<std::string>("--to-file") : "";


    if (hashList.empty() && hashValue.empty()) {
        std::cerr << "Either --hash-file or --hash-value must be provided." << std::endl;
        return 1;
    }

    bool notSave = toFile.empty();


    Attack *attack = nullptr;
    std::string outputString;

    auto modeStr = program.get<std::string>("mode");
    Mode mode = parseMode(modeStr);

    switch (mode) {
        case Mode::Dictionary: {
            try {
                if (!wordList.empty() && !hashList.empty()) {
                    attack = new DictionaryAttack(hashList, wordList, DataSource::FILE_PATHS, hashType);
                } else if (!hashList.empty() && !word.empty()) {
                    attack = new DictionaryAttack(hashList, word, DataSource::HASH_LIST, hashType);
                } else if (!wordList.empty() && !hashValue.empty()) {
                    attack = new DictionaryAttack(hashValue, wordList, DataSource::WORD_LIST, hashType);
                } else if (!word.empty()) {
                    attack = new DictionaryAttack(hashValue, word, DataSource::STRINGS, hashType);
                } else {
                    std::cerr << "Either --wordlist or --word must be provided." << std::endl;
                    return 1;
                }
            } catch (const std::exception &err) {
                std::cerr << err.what() << std::endl;
            }
            break;
        }
        case Mode::Brute: {
            try {
                if (!hashList.empty()) {
                    attack = new BruteForce(hashList, DataSource::HASH_LIST, hashType, maxLen, charset);
                } else {
                    attack = new BruteForce(hashValue, DataSource::STRINGS, hashType, maxLen, charset);
                }
            } catch (const std::exception &err) {
                std::cerr << err.what() << std::endl;
            }
            break;
        }
        case Mode::Identify: {
            try {
                std::stringstream outputStringStream;

                if (notSave) {
                    std::cout << fmt::format(fg(fmt::color::beige), "HASH") << "    |    ";
                    std::cout << fmt::format(fg(fmt::color::red), "PROBABLE HASH ALGORITHM") << std::endl << std::endl;
                }

                if (!hashList.empty()) {
                    HashIdentifier identifier = HashIdentifier(hashList);
                    if (verbose) {
                        outputStringStream << identifier.toStringVerbose();
                    } else {
                        outputStringStream << identifier.toString();
                    }
                } else {
                    std::unordered_set<std::string> set;
                    set.insert(hashValue);
                    GroupedHashes hash = GroupedHashes(set);

                    if (notSave) {
                        if (verbose) {
                            outputStringStream << hash.toStringVerbose();
                        } else {
                            outputStringStream << hash.toString();
                        }
                    } else {
                        if (verbose) {
                            outputStringStream << hash.forFileVerbose();
                        } else {
                            outputStringStream << hash.forFile();
                        }
                    }
                }
                outputString = outputStringStream.str();
            } catch (const std::exception &err) {
                std::cerr << err.what() << std::endl;
            }
            break;
        }

        case Mode::Unknown:
        default: {

            std::cerr << "Provided mode \"" << modeStr << "\" is invalid." << std::endl;
            std::cerr << program;
            return 1;
        }
    }


    if (attack != nullptr) {
        attack->startAttack(numThreads);
        std::vector<BrokenHash> brokenHashes = attack->getBrokenHashes();
        std::stringstream outputStringStream;


        if (!brokenHashes.empty()) {
            if (notSave) {
                outputStringStream << fmt::format("{} | {} | {}",
                                                  fmt::format(fg(fmt::color::beige), "HASH"),
                                                  fmt::format(fg(fmt::color::green_yellow), "PLAIN TEXT"),
                                                  fmt::format(fg(fmt::color::red), "HASH ALGORITHM")) <<
                                   std::endl << std::endl;

                for (const auto &brokenHash: brokenHashes) {
                    outputStringStream << brokenHash.toString();
                }
            } else {
                for (const auto &brokenHash: brokenHashes) {
                    outputStringStream << brokenHash.forFile();
                }
            }
        } else {
            std::cout << fmt::format(fg(fmt::color::red), "NO HASH BROKEN");
        }

        outputString = outputStringStream.str();
    }


    if (notSave) {
        std::cout << outputString;
    } else {
        FileManager::saveOutput(toFile, outputString);
    }

    return 0;
}
