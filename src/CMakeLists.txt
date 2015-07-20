cmake_minimum_required (VERSION 2.8)

# All CPP and HPP source files.
set(SRC
   main.cpp

# Misc and tools section	   		
   BlockCipherModes.cpp
   Base64.cpp
   SquareMatrix.cpp
   CipherGrid.cpp
   MathematicalTools.cpp
	Tools.cpp
   String.cpp
   Padding.cpp

# Cryptanalysis section
   #cryptanalysis/ClassicalAnalysis.cpp
	
# Classic ciphers section
   StringCipher.cpp
   StringCipherWithStringKey.cpp
   StringCipherWithPermutationKey.cpp
	Adfgvx.cpp
	Collon.cpp
	Delastelle.cpp
	Fleissner.cpp
	Hill.cpp
   Transposition.cpp
	Morse.cpp
	Playfair.cpp
	Polybius.cpp
	Railfence.cpp
   UnsortAlpha.cpp
	Vigenere.cpp
	Wolseley.cpp
   Affine.cpp
   MonoalphabeticSubstitution.cpp
   Nihilistes.cpp
   Chao.cpp
   TwoSquares.cpp
   ThreeSquares.cpp
   FourSquares.cpp

# Block ciphers section
   TripleDES.cpp
   AES.cpp
   IDEA.cpp
   Noekeon.cpp
   RC2.cpp
   #Serpent.cpp
   Skipjack.cpp
   Blowfish.cpp
   DES.cpp
   RC5.cpp
   RC6.cpp
   XTEA.cpp
   CAST128.cpp
   CAST256.cpp
   Twofish.cpp
   Camellia.cpp
   Mysty1.cpp

# Stream ciphers and Pseudo-random functions familly section
   RC4.cpp
   Rabbit.cpp
   HC256.cpp
   SEAL.cpp
   Isaac.cpp
   Scream.cpp
   Salsa20.cpp
   Snow3G.cpp

# Hash functions section
   MessageDigest.cpp
   MD2.cpp
   SHA2.cpp
   SHA1.cpp
   Ripemd.cpp
   Tiger.cpp
   Whirlpool.cpp
   Blake.cpp

# Sponge functions section
   Keccak.cpp

# Checksum functions
   LRC.cpp
   Adler32.cpp

# Asymmetric ciphers section
   AsymmetricCipher.cpp
   HellmanMerkleKnapsack.cpp
)

# exception handlers section
include_directories (exceptions)

# Add the source files code (CPP, HPP and the CMakeLists.txt) of 'big_numbers' directory.
add_subdirectory (big_integers)

# Create libCryptoGL.a with all the source files provided by SRC.
add_library(cryptoGL ${SRC})


