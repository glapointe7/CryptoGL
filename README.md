
# Introduction

CryptoGL is a C++ library containing classical ciphers, symmetric ciphers, asymmetric ciphers, hash functions, sponge functions, checksum functions and some other tools for strings, vectors, grids, manipulation of bits and mathematical purpuses. All the CPP and HPP files are found in the `src` folder.

The `UML` folder contains the UML class diagrams of this library. Each DIA file is represented by a main classes to make the UML diagram more readable. The DIA files have to be open with the freeware DIA found [here](http://dia-installer.de/) for Windows users. For linux users, `sudo apt-get install dia` and it's done.

The `UnitTests` folder contains the unit tests framework and all tests done for the cryptographic algorithms and some other tools. Each algorithm contains tests in a HPP file and each test is a child class. You will see many examples on how to use them. A report is generated for all tests performed (have a look in main.cpp file).

# Compilation

CryptoGL has been compiled successfully using netbeans 8.0.2 with clang++ 3.6 which is using some features of C++11 and C++14.

To compile with Clang++ 3.6 and using c++14, use the option `-std=c++1z`.
Also, in the cstdio file, replace `using ::gets` by `#if !_ISOC11_SOURCE using ::gets; #endif`.


# Cryptographic algorithms

#### Classical ciphers

* ADFGVX
* Caesar
* Vigenere, Rozier, Beaufort, Beaufort (German variant) and Vigenere Multiplicative
* Chao
* Collon
* Delastelle
* Fleissner
* 2-square, 3-square and 4-square
* Hill
* Morse
* Monoalphabetic Substitution
* Nihiliste
* Playfair
* Polibius
* Railfence, Redefence and Redefence Zigzag
* Transpositions
* Wolseley
* Affine

#### Block ciphers

* AES
* DES
* Blowfish
* IDEA
* Skipjack
* RC2, RC5 and RC6
* TripleDES
* XTEA
* Noekeon
* CAST-128 and CAST-256
* Twofish
* Camellia
* MYSTY1

For each block cipher, the ECB, CBC, CFB, OFB and CTR modes of operation are implemented following the [specifications of NIST](http://csrc.nist.gov/groups/ST/toolkit/BCM/index.html).

#### Stream ciphers and Pseudo-random functions

* Rabbit
* RC4
* SEAL
* ISAAC
* HC-256
* Salsa20
* Scream-S, Scream-0
* Snow3G

#### Asymmetric ciphers 

* Hellman-Merkle Knapsack cryptosystem, RSA (to come)

These algorithms are [using a BigInteger library created by Matt McCutchen](https://mattmccutchen.net/bigint/)

#### Hash functions

* Blake (224, 256, 384, 512)
* SHA-1
* SHA-2 (224, 256, 384, 512, 512/224, 512/256)
* MD2, MD4, MD5
* RipeMD (128, 160, 256, 320)
* Whirlpool
* Tiger and tiger2 (128, 160, 192)
* HMAC is supported for all of them.

#### Sponge construction functions

* Keccak (0, 224, 256, 384, 512)

#### Checksum functions

* LRC (Longitudinal redundancy check)
* Adler-32

#### MAC algorithms

* CBC-MAC
* AES-XCBC-MAC
* CMAC
* PMAC
* OMAC
* TMAC
* HMAC

#### Other tools

* Base64
* Square matrices over the modular integers $\mathbb{Z}_n$
* Functions composition 
* Big and little endian conversion 
* Various mathematical tools
* Vectors extending the STL vectors
* Strings extending the STL strings
* Padding Schemes 
* Bits manipulation tools


# Execution with CMake

Here is the procedure to execute the tests in the terminal with cmake:

1. `mkdir Build`
2. `cd Build`
3. `cmake ..`
4. `make CryptoGL`
5. (To get the result in a file): `./CryptoGL | perl -pe 's/\e\[?.*?[\@-~]//g' &> CryptoGL_TestsReport.txt`
6. (To get the result on terminal): `./CryptoGL`
