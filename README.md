# Cracker++

This is a simple and fast command-line password cracker. 
It handle some of the most popular hash types. It also includes functionality to identify them.
## Requiremenets: 
Before building and running Cracker++, ensure you have the following dependencies installed:

- [cmake](https://cmake.org/)
- [OpenSSL](https://www.openssl.org/)
- [fmt](https://github.com/fmtlib/fmt) 
- [argparse](https://github.com/p-ranav/argparse) 

- - -

## Build Instructions

Follow these steps to build **Cracker++** on an **Unix** system:

1. **Clone the repository**:

   ```bash
   git clone https://github.com/MKowalski8/CrackerPP.git
   ```

1. **Navigate to the project directory:**
    ``` bash
    cd CrackerPP
    ```

1. **Create and navigate to a build directory:**
    ``` bash
    mkdir build && cd build
    ```

1. **Build the project**
    ```bash
    cmake .. && cmake --build .
    ```

- - - 
## How to use 
After running ``./Cracker++`` you would see this help meessage
```
./Cracker++
mode: 1 argument(s) expected. 0 provided.
Usage: CRACKER++ [--help] [--version] [--hash-file VAR] [--hash-value VAR] [--wordlist VAR] [--word VAR] [--charset VAR] [--max-len VAR] [--threads VAR] [--hash-type VAR] [--verbose] mode

Positional arguments:
  mode              Mode: dictionary, brute or identifier 

Optional arguments:
  -h, --help        shows help message and exits 
  -v, --version     prints version information and exits 
  -hl, --hash-file  File containing hashes 
  --hash-value      Single hash value 
  -w, --wordlist    Path to wordlist file 
  -sw, --word       Single word 
  --charset         Character set for brute-force 
  -l, --max-len     Maximum length of passwords to try [nargs=0..1] [default: 8]
  -t, --threads     Number of threads to use [nargs=0..1] [default: 1]
  -ht, --hash-type  Type of hash algorithm (e.g., sha256, md5, sha1) 
  -vb, --verbose    Write all of the possible hash algorithms while identifying   
  -o, --to-file     Save output to file
```

**To use it you have to precise a mode you want to use**

### Dictionary Attack
To perform a dictionary attack you have to provide:
-  **wordlist** / **single word**
- **hashlist** / **single hash**

### Brute Force
This mode needs only a **hashlist** / **single hash**. 

You are also able to set: 
- charset 
- max length of generated word

### Hash Identifier
If you only want to check which probable hash algorithms where used with your hashes, this mode is for you.

It can be run with ``-vb`` flag, which gives you also least possbile types

### Examples:

Dictioanry attack using wordlist and hashlist:
```
./Cracker++ dictionary -w passwords.txt -hl hashes.txt 
```
Brute force attack on one hash:
```
./Cracker++ brute --hash-value 0cc175b9c0f1b6a831c399e269772661 -l 4
```
Identify hashes:
```
./Cracker++ identify -hl hashes.txt -vb
```
