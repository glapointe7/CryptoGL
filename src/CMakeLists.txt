cmake_minimum_required (VERSION 2.8)

# All CPP and HPP source files.
set(SRC
   main.cpp
	Cipher.hpp
   HashFunction.hpp HashFunction.cpp

	StringCipher.hpp StringCipher.cpp
	SquareCipher.hpp SquareCipher.cpp
   StringCipherWithStringKey.hpp StringCipherWithStringKey.cpp
   StringCipherWithPermutationKey.hpp StringCipherWithPermutationKey.cpp

	StreamCipher.hpp StreamCipher.cpp	
	AsymmetricCipher.hpp AsymmetricCipher.cpp
	SymmetricCipher.hpp SymmetricCipher.cpp
	
   Matrix.hpp Matrix.cpp
	ConverterTools.hpp ConverterTools.cpp
   MathematicalTools.hpp MathematicalTools.cpp
	LittleEndian.hpp
   BlockCipherOperationModes.hpp
	Tools.hpp Tools.cpp
	Transposition.hpp Transposition.cpp
   String.hpp String.cpp
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
	Fleissner.hpp Fleissner.cpp
	Hellman.hpp Hellman.cpp
	Hill.hpp Hill.cpp
   MessageDigest.hpp MessageDigest.cpp
   SHA.hpp SHA.cpp
   Ripemd.hpp Ripemd.cpp
	Morse.hpp Morse.cpp
	Playfair.hpp Playfair.cpp
	Polybe.hpp Polybe.cpp
	Rabbit.hpp Rabbit.cpp
	Railfence.hpp Railfence.cpp
	RC4.hpp RC4.cpp
	UnsortAlpha.hpp UnsortAlpha.cpp
	Vigenere.hpp Vigenere.cpp
	Wolseley.hpp Wolseley.cpp
   Affine.hpp Affine.cpp
   Blowfish.hpp Blowfish.cpp
   MonoalphabeticSubstitution.hpp MonoalphabeticSubstitution.cpp
   BlockCipher.hpp BlockCipher.cpp
   Feistel.hpp Feistel.cpp
   BlockCipherStrategy.hpp BlockCipherStrategy.cpp
   Base64.hpp Base64.cpp
   Nihilistes.hpp Nihilistes.cpp
   Chao.hpp Chao.cpp
   TwoSquares.hpp TwoSquares.cpp
   ThreeSquares.hpp ThreeSquares.cpp
   FourSquares.hpp FourSquares.cpp
)

# Create libcomponants.a with all the source files provided by SRC.
add_library(componants ${SRC})


