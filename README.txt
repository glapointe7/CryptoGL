Cryptology is a c++ library containing classical ciphers, modern ciphers and hash functions.

Classical ciphers include the following :
ADFGVX, Caesar, Vigenere, Rozier, Beaufort, Beaufort (German variant), Vigenere Multiplicative, Chao, Collon, Delastelle, Fleissner, 2-square, 3-square, 4-square, Hill, Morse, Monoalphabetic Substitution, Nihiliste, Playfair, Polibius, Railfence, Redefence, Transpositions, Wolseley and Affine.

Block ciphers include the following :
AES, DES, Blowfish, IDEA, Skipjack, RC2, RC5, RC6, TripleDES, XTEA, Noekeon, CAST-128, CAST-256

Stream ciphers include the following :
Rabbit, RC4

Hash functions include the following :
Blake (224, 256, 384, 512), Keccak (0, 224, 256, 384, 512), SHA-1, SHA-2 (224, 256, 384, 512, 512/224, 512/256), MD2, MD4, MD5, RipeMD (128, 160, 256, 320), Whirlpool, Tiger (128, 160, 192). HMAC is supported for all of them except Keccak which it's not needed.

Asymmetric ciphers include the following :
Hellman & Merkle

Other tools :
Base64, Matrix over the modular integers (Z_n), Big and little endian conversion, various mathematical and bits manipulation tools.

Also, at least one test is done for each cipher and hash function. The tests are done with GoogleTest functionalities.

*****MORE TO COME*****
- Add algorithm to generate random strings, bytes and bits following the NIST standard.
- Add RSA cipher.
- Add CBC, CFB, OFB and CTR modes for block cipher.
- Add more tests for each crypto.
**********************

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
