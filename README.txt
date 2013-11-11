CryptoGL is a c++ library containing classical ciphers, symmetric ciphers, asymmetric ciphers, hash functions and some other tools. The UML directory contains the UML class diagrams of this library.
CryptoGL has been compiled successfully with g++ 4.7 and clang++ 3.3 and uses some features of C++11. 

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


Other tools :
Base64, square matrices over the modular integers (Z_n), Functions composition, Big and little endian conversion, various mathematical, padding and bits manipulation tools.

Also, at least one test is done for each cipher and hash function. The tests are done with GoogleTest functionalities.



1. Procedure to install GoogleTest (gtest) on your computer ( http://code.google.com/p/tonatiuh/wiki/InstallingGoogleTestForLinux ):

tar -xzvf gtest-1.6.0.tar.gz

cd gtest-1.6.0
mkdir mybuild
cd mybuild
cmake -G"Unix Makefiles" ..
make

cp -r ../include/gtest /usr/local/include
cp lib*.a /usr/local/lib

********************************************************************

2. If you are using NetBeans :

Right click on your project name and choose PROPERTIES.
Under Categories at your left, select C++ COMPILER.
At your right, in the ADDITIONAL OPTIONS, add '-lgtest -pthread' without the quotes. This will link to the gtest and gtest_main libraries.

******************************************************************** 
