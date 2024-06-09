cmake_minimum_required(VERSION 3.29)
project(Cracker++)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenSSL REQUIRED)
find_package(fmt REQUIRED)

add_executable(Cracker++ main.cpp
        hashes/hash_util.h
        hashes/hash_util.cpp
        attacks/dictionary.cpp
        attacks/dictionary.h
        hashes/broken_hash.cpp
        hashes/broken_hash.h
        attacks/attack.cpp
        attacks/attack.h
        attacks/brutte_force.cpp
        attacks/brutte_force.h
        file_loader.cpp
        file_loader.h
        hashes/hash_identifier.cpp
        hashes/hash_identifier.h
        hashes/grouped_hashes.cpp
        hashes/grouped_hashes.h
        attacks/DataSource.h
)

target_link_libraries(Cracker++ OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(Cracker++  fmt::fmt)