#add_library(Classical INTERFACE)
#add_library(cryptoGL::Classical ALIAS Classical)
add_subdirectory(common)
add_subdirectory(adfgvx)
add_subdirectory(affine)
add_subdirectory(chao)
add_subdirectory(collon)
add_subdirectory(delastelle)
add_subdirectory(fleissner)
add_subdirectory(four_squares)
add_subdirectory(hill)
add_subdirectory(monoalphabetic_substitution)
add_subdirectory(morse)
add_subdirectory(nihilistes)
add_subdirectory(playfair)
add_subdirectory(polybius)
add_subdirectory(railfence)
add_subdirectory(three_squares)
add_subdirectory(transposition)
add_subdirectory(two_squares)
add_subdirectory(unsort_alpha)
add_subdirectory(vigenere)
add_subdirectory(wolseley)


# Set include directories for the interface library
# target_include_directories(Classical INTERFACE 
#     ${CMAKE_CURRENT_SOURCE_DIR}
#     ${CMAKE_CURRENT_SOURCE_DIR}/common
# )

# target_link_libraries(Classical INTERFACE
#     cryptoGL::Core
#     cryptoGL::ClassicalCommon
#     cryptoGL::Adfgvx
#     cryptoGL::Affine
#     cryptoGL::Chao
#     cryptoGL::Collon
#     cryptoGL::Delastelle
#     cryptoGL::Fleissner
#     cryptoGL::FourSquares
#     cryptoGL::Hill
#     cryptoGL::MonoalphabeticSubstitution
#     cryptoGL::Morse
#     cryptoGL::Nihilistes
#     cryptoGL::Playfair
#     cryptoGL::Polybius
#     cryptoGL::Railfence
#     cryptoGL::ThreeSquares
#     cryptoGL::Transposition
#     cryptoGL::TwoSquares
#     cryptoGL::UnsortAlpha
#     cryptoGL::Vigenere
#     cryptoGL::Wolseley
# )