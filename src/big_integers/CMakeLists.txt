cmake_minimum_required (VERSION 3.8)

# All CPP and HPP source files.
set(SRC_BigInt
   NumberlikeArray.hh
	BigUnsigned.hh BigUnsigned.cc
	BigInteger.hh BigInteger.cc
	BigIntegerAlgorithms.hh BigIntegerAlgorithms.cc
	BigUnsignedInABase.hh BigUnsignedInABase.cc
	BigIntegerLibrary.hh
   BigIntegerUtils.hh BigIntegerUtils.cc
)

# Create libcomponants.a with all the source files provided by SRC.
add_library(big_integers ${SRC_BigInt})
