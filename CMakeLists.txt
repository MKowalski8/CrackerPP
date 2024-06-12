cmake_minimum_required(VERSION 3.29)
project(Cracker++)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenSSL REQUIRED)
find_package(fmt REQUIRED)
find_package(argparse REQUIRED)

add_executable(Cracker++ main.cpp
        hashes/hash_util.h
        hashes/hash_util.cpp
        attacks/dictionary.cpp
        attacks/dictionary.h
        hashes/broken_hash.cpp
        hashes/broken_hash.h
        attacks/attack.cpp
        attacks/attack.h
        attacks/brute_force.cpp
        attacks/brute_force.h
        file_manager.cpp
        file_manager.h
        hashes/hash_identifier.cpp
        hashes/hash_identifier.h
        hashes/grouped_hashes.cpp
        hashes/grouped_hashes.h
        attacks/data_source.h
        mode.h
)

target_link_libraries(Cracker++
        OpenSSL::SSL
        OpenSSL::Crypto
        fmt::fmt
        argparse::argparse
)