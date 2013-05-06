cmake_minimum_required (VERSION 2.8)

# Prepare all testing files to execute.
set(Testing
	CaesarTest.cpp
   VigenereTest.cpp
   RozierTest.cpp
   BeaufortTest.cpp
   BeaufortGermanTest.cpp
   PlayfairTest.cpp
   PolybeTest.cpp
   CollonTest.cpp
   AdfgvxTest.cpp
   DelastelleTest.cpp
   RailfenceTest.cpp
   WolseleyTest.cpp
   SHA1Test.cpp
   HellmanTest.cpp
   TranspositionRowsTest.cpp
   TranspositionColumnsTest.cpp
   TranspositionDoubleTest.cpp
   MorseTest.cpp
   UnsortAlphaTest.cpp
   DESTest.cpp
   RC4Test.cpp
)

add_library(test_classes ${Testing})

