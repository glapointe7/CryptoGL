CryptoGL is a c++ library containing classical ciphers, symmetric ciphers, asymmetric ciphers, hash functions and some other tools. 
The UML directory contains the UML class diagrams of this library.
CryptoGL has been compiled successfully using netbeans 8.0.2 with g++ 4.8 and clang++ 3.6 which are using features of C++14.
*************************************************
To compile with Clang++ 3.6 and using c++14, use the option -std=c++1z
Also, in the cstdio file, replace `using ::gets` by #if !_ISOC11_SOURCE using ::gets; #endif
*************************************************
 

Classical ciphers include the following :
ADFGVX, Caesar, Vigenere, Rozier, Beaufort, Beaufort (German variant), Vigenere Multiplicative, Chao, Collon, Delastelle, Fleissner, 2-square, 3-square, 4-square, Hill, Morse, Monoalphabetic Substitution, Nihiliste, Playfair, Polibius, Railfence, Redefence, Transpositions, Wolseley and Affine.

Block ciphers include the following :
AES, DES, Blowfish, IDEA, Skipjack, RC2, RC5, RC6, TripleDES, XTEA, Noekeon, CAST-128, CAST-256, Twofish, Camellia, MYSTY1

For each block cipher, the ECB, CBC, CFB, OFB and CTR modes of operation are implemented following the specifications of NIST.

Stream ciphers and/or Pseudo-random functions familly include the following :
Rabbit, RC4, SEAL, ISAAC, HC-256, Salsa20, Scream-S, Scream-0, Snow3G

Asymmetric ciphers include the following (using a BigInteger library created by Matt McCutchen : https://mattmccutchen.net/bigint/):
Hellman-Merkle Knapsack cryptosystem

Hash functions include the following :
Blake (224, 256, 384, 512), SHA-1, SHA-2 (224, 256, 384, 512, 512/224, 512/256), MD2, MD4, MD5, RipeMD (128, 160, 256, 320), Whirlpool, Tiger and tiger2 (128, 160, 192). HMAC is supported for all of them.

Sponge construction functions include the following :
Keccak (0, 224, 256, 384, 512)

Checksum functions include the following :
LRC (Longitudinal redundancy check), Adler-32

MAC algorithms include the following :
CBC-MAC, AES-XCBC-MAC, CMAC, PMAC, OMAC, TMAC and HMAC

Other tools :
Base64, square matrices over the modular integers (Z_n), Functions composition, Big and little endian conversion, various mathematical, vector, padding and bits manipulation tools.

Unit tests framework:
CryptoGL contains his own unit tests framework. The folder 'UnitTests' contains a lot of tests as examples. A report is generated for all tests.


*******************************************************************************
To execute in terminal with cmake:

1. mkdir Build
2. cd Build
3. cmake ..
4. make CryptoGL

5. (To get the result in a file): ./CryptoGL | perl -pe 's/\e\[?.*?[\@-~]//g' &> CryptoGL_TestsReport.txt
6. (To get the result on terminal): ./CryptoGL

*******************************************************************************
