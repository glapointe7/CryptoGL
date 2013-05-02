cmake_minimum_required (VERSION 2.8)
project (Cryptology_exe)

# http://stackoverflow.com/questions/6352123/multiple-directories-under-cmake
set(SRC
	main.cpp
	Cipher.hpp

	StringCipher.hpp StringCipher.cpp
	SquareCipher.hpp SquareCipher.cpp

	HashFunction.hpp HashFunction.cpp
	StreamCipher.hpp StreamCipher.cpp	
	AsymmetricCipher.hpp AsymmetricCipher.cpp
	SymmetricCipher.hpp SymmetricCipher.cpp
	
   Matrix.hpp Matrix.cpp
	converterTools.hpp converterTools.cpp
   MathematicalTools.hpp MathematicalTools.cpp
	LittleIndian.hpp
	Tools.hpp Tools.cpp
	Transposition.hpp Transposition.cpp

   exceptions/Exception.hpp
   exceptions/BadAlpha.hpp exceptions/BadAlpha.cpp
   exceptions/BadLength.hpp exceptions/BadLength.cpp
   exceptions/BadKey.hpp exceptions/BadKey.cpp
   exceptions/BadKeyLength.hpp exceptions/BadKeyLength.cpp
   exceptions/BadChar.hpp exceptions/BadChar.cpp
   exceptions/EmptyAlpha.hpp exceptions/EmptyAlpha.cpp
   exceptions/EmptyKey.hpp exceptions/EmptyKey.cpp
   exceptions/MatrixKeyNotReversible.hpp exceptions/MatrixKeyNotReversible.cpp
   exceptions/BadMatrix.hpp exceptions/BadMatrix.cpp
   exceptions/EmptyMatrix.hpp exceptions/EmptyMatrix.cpp
   exceptions/MatrixNotSquare.hpp exceptions/MatrixNotSquare.cpp
   exceptions/MatrixOutOfRange.hpp exceptions/MatrixOutOfRange.cpp
   exceptions/EmptyGridKey.hpp exceptions/EmptyGridKey.cpp
   exceptions/BadGridDimension.hpp exceptions/BadGridDimension.cpp
   exceptions/KeyNotSuperIncreasing.hpp exceptions/KeyNotSuperIncreasing.cpp
   exceptions/IntegersNotCoprime.hpp exceptions/IntegersNotCoprime.cpp
   exceptions/BadModulo.hpp exceptions/BadModulo.cpp
	
	Adfgvx.hpp Adfgvx.cpp
	Collon.hpp Collon.cpp
	Delastelle.hpp Delastelle.cpp
	DES.hpp DES.cpp
	Fleissner.hpp Fleissner.cpp
	Hellman.hpp Hellman.cpp
	#Hill.hpp Hill.cpp
	MD5.hpp MD5.cpp
	SHA1.hpp SHA1.cpp
	Morse.hpp Morse.cpp
	Playfair.hpp Playfair.cpp
	Polybe.hpp Polybe.cpp
	Rabbit.hpp Rabbit.cpp
	Railfence.hpp Railfence.cpp
	RC4.hpp RC4.cpp
	UnsortAlpha.hpp UnsortAlpha.cpp
	Vigenere.hpp Vigenere.hpp
	Wolseley.hpp Wolseley.cpp
)

#add_library(Cryptology ${SRC})
add_executable(Cryptology ${SRC})


