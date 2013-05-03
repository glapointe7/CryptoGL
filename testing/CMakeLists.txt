cmake_minimum_required (VERSION 2.8)
project (Cryptology_exe)

# http://stackoverflow.com/questions/6352123/multiple-directories-under-cmake
set(testing
	main.cpp
	CaesarTest.hpp
   VigenereTest.hpp
   RozierTest.hpp
   BeaufortTest.hpp
   BeaufortGermanTest.hpp
   PlayfairTest.hpp
   PolybeTest.hpp
   CollonTest.hpp
   AdfgvxTest.hpp
)

#add_library(Cryptology ${testing})
add_executable(Cryptology ${testing})


