cmake_minimum_required (VERSION 2.8)

# All CPP and HPP source files.
set(SRC
   main.cpp
	Cipher.hpp
   HashFunction.hpp

	StringCipher.hpp StringCipher.cpp
	SquareCipher.hpp SquareCipher.cpp
   StringCipherWithStringKey.hpp StringCipherWithStringKey.cpp
   StringCipherWithPermutationKey.hpp StringCipherWithPermutationKey.cpp

	StreamCipher.hpp StreamCipher.cpp	
	AsymmetricCipher.hpp AsymmetricCipher.cpp
	SymmetricCipher.hpp
	
   SquareMatrix.hpp SquareMatrix.cpp
	ConverterTools.hpp ConverterTools.cpp
   MathematicalTools.hpp MathematicalTools.cpp
	Endianness.hpp
   BigEndian.hpp
   LittleEndian.hpp
   BlockCipherOperationModes.hpp
	Tools.hpp Tools.cpp
	Transposition.hpp Transposition.cpp
   String.hpp String.cpp
   Digest.hpp
   File.hpp
   Bits.hpp
   Random.hpp Random.cpp

   exceptions/Exception.hpp
   exceptions/BadAlpha.hpp
   exceptions/BadAlphaLength.hpp
   exceptions/BadKey.hpp
   exceptions/BadKeyLength.hpp
   exceptions/BadChar.hpp
   exceptions/EmptyAlpha.hpp
   exceptions/EmptyKey.hpp
   exceptions/MatrixKeyNotReversible.hpp
   exceptions/BadMatrix.hpp
   exceptions/EmptyMatrix.hpp
   exceptions/MatrixNotSquare.hpp
   exceptions/MatrixOutOfRange.hpp
   exceptions/EmptyGridKey.hpp
   exceptions/BadGridDimension.hpp
   exceptions/IntegersNotCoprime.hpp
   exceptions/BadModulo.hpp

   cryptanalysis/ClassicalAnalysis.hpp cryptanalysis/ClassicalAnalysis.cpp
	
	Adfgvx.hpp Adfgvx.cpp
	Collon.hpp Collon.cpp
	Delastelle.hpp Delastelle.cpp
	DES.hpp DES.cpp
   TripleDES.hpp TripleDES.cpp
   AES.hpp AES.cpp
   IDEA.hpp IDEA.cpp
   Skipjack.hpp Skipjack.cpp
   Noekeon.hpp Noekeon.cpp
	Fleissner.hpp Fleissner.cpp
	Hill.hpp Hill.cpp
   MessageDigest.hpp MessageDigest.cpp
   SHA2.hpp SHA2.cpp
   SHA1.hpp SHA1.cpp
   SpongeFunction.hpp
   Keccak.hpp Keccak.cpp
   Ripemd.hpp Ripemd.cpp
   Tiger.hpp Tiger.cpp
   Whirlpool.hpp Whirlpool.cpp
   Blake.hpp Blake.cpp
	Morse.hpp Morse.cpp
	Playfair.hpp Playfair.cpp
	Polybius.hpp Polybius.cpp
	Rabbit.hpp Rabbit.cpp
	Railfence.hpp Railfence.cpp

   RC2.hpp RC2.cpp
	RC4.hpp RC4.cpp
   RC5.hpp RC5.cpp
   RC6.hpp RC6.cpp
   XTEA.hpp XTEA.cpp
   CAST128.hpp CAST128.cpp
   CAST256.hpp CAST256.cpp
   Twofish.hpp Twofish.cpp 

	UnsortAlpha.hpp UnsortAlpha.cpp
	Vigenere.hpp Vigenere.cpp
	Wolseley.hpp Wolseley.cpp
   Affine.hpp Affine.cpp
   Blowfish.hpp Blowfish.cpp
   MonoalphabeticSubstitution.hpp MonoalphabeticSubstitution.cpp
   BlockCipher.hpp
   Feistel.hpp
   BlockCipherModes.hpp BlockCipherModes.cpp
   Base64.hpp Base64.cpp
   Nihilistes.hpp Nihilistes.cpp
   Chao.hpp Chao.cpp
   TwoSquares.hpp TwoSquares.cpp
   ThreeSquares.hpp ThreeSquares.cpp
   FourSquares.hpp FourSquares.cpp

   HellmanMerkleKnapsack.hpp HellmanMerkleKnapsack.cpp
)

# Add the source files code (CPP, HPP and the CMakeLists.txt) of 'big_numbers' directory.
add_subdirectory (big_integers)

# Create libcomponants.a with all the source files provided by SRC.
add_library(cryptoGL ${SRC})


