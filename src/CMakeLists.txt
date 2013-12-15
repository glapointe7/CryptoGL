cmake_minimum_required (VERSION 2.8)

# All CPP and HPP source files.
set(SRC
   main.cpp

# Misc and tools section	   		
   BlockCipherModes.hpp BlockCipherModes.cpp
   MAC.hpp
   Base64.hpp Base64.cpp
   SquareMatrix.hpp SquareMatrix.cpp
   MathematicalTools.hpp MathematicalTools.cpp
   BigEndian.hpp
   LittleEndian.hpp
	Tools.hpp Tools.cpp
   String.hpp String.cpp
   Digest.hpp
   File.hpp
   Bits.hpp Bits.cpp
   Types.hpp
   Vector.hpp Vector.cpp
   Padding.hpp Padding.cpp
   FunctionComposition.hpp

# Cryptanalysis section
   cryptanalysis/ClassicalAnalysis.hpp cryptanalysis/ClassicalAnalysis.cpp
	
# Classic ciphers section
   Cipher.hpp
   SymmetricCipher.hpp
   StringCipher.hpp StringCipher.cpp
   SquareCipher.hpp SquareCipher.cpp
   StringCipherWithStringKey.hpp StringCipherWithStringKey.cpp
   StringCipherWithPermutationKey.hpp StringCipherWithPermutationKey.cpp
	Adfgvx.hpp Adfgvx.cpp
	Collon.hpp Collon.cpp
	Delastelle.hpp Delastelle.cpp
	Fleissner.hpp Fleissner.cpp
	Hill.hpp Hill.cpp
   Transposition.hpp Transposition.cpp
	Morse.hpp Morse.cpp
	Playfair.hpp Playfair.cpp
	Polybius.hpp Polybius.cpp
	Railfence.hpp Railfence.cpp
   UnsortAlpha.hpp UnsortAlpha.cpp
	Vigenere.hpp Vigenere.cpp
	Wolseley.hpp Wolseley.cpp
   Affine.hpp Affine.cpp
   MonoalphabeticSubstitution.hpp MonoalphabeticSubstitution.cpp
   Nihilistes.hpp Nihilistes.cpp
   Chao.hpp Chao.cpp
   TwoSquares.hpp TwoSquares.cpp
   ThreeSquares.hpp ThreeSquares.cpp
   FourSquares.hpp FourSquares.cpp

# Block ciphers section
   BlockCipher.hpp
   TripleDES.hpp TripleDES.cpp
   AES.hpp AES.cpp
   IDEA.hpp IDEA.cpp
   Noekeon.hpp Noekeon.cpp
   RC2.hpp RC2.cpp
   Serpent.hpp Serpent.cpp
   Feistel.hpp
   Skipjack.hpp Skipjack.cpp
   Blowfish.hpp Blowfish.cpp
   DES.hpp DES.cpp
   RC5.hpp RC5.cpp
   RC6.hpp RC6.cpp
   XTEA.hpp XTEA.cpp
   CAST128.hpp CAST128.cpp
   CAST256.hpp CAST256.cpp
   Twofish.hpp Twofish.cpp
   Camellia.hpp Camellia.cpp
   Mysty1.hpp Mysty1.cpp

# Stream ciphers and Pseudo-random functions familly section
   StreamCipher.hpp
   SynchronousStreamCipher.hpp
   PseudoRandomFunction.hpp
   RC4.hpp RC4.cpp
   Rabbit.hpp Rabbit.cpp
   HC256.hpp HC256.cpp
   SEAL.hpp SEAL.cpp
   Isaac.hpp Isaac.cpp
   Scream.hpp Scream.cpp
   Salsa20.hpp Salsa20.cpp
   Snow3G.hpp Snow3G.cpp

# Hash functions section
   MerkleDamgardFunction.hpp
   HashFunction.hpp
   MessageDigest.hpp MessageDigest.cpp
   MD2.hpp MD2.cpp
   SHA2.hpp SHA2.cpp
   SHA1.hpp SHA1.cpp
   Ripemd.hpp Ripemd.cpp
   Tiger.hpp Tiger.cpp
   Whirlpool.hpp Whirlpool.cpp
   Blake.hpp Blake.cpp

# Sponge functions section
   SpongeFunction.hpp
   Keccak.hpp Keccak.cpp

# Checksum functions
   Checksum.hpp
   LRC.hpp LRC.cpp
   Adler32.hpp Adler32.cpp

# Asymmetric ciphers section
   AsymmetricCipher.hpp AsymmetricCipher.cpp
   HellmanMerkleKnapsack.hpp HellmanMerkleKnapsack.cpp
)

# exception handlers section
include_directories (exceptions)

# Add the source files code (CPP, HPP and the CMakeLists.txt) of 'big_numbers' directory.
add_subdirectory (big_integers)

# Create libcomponants.a with all the source files provided by SRC.
add_library(cryptoGL ${SRC})


