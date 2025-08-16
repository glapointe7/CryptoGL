# Add the subdirectories only to tell the Algorithms library 
# that there are another level of subdirectories.
add_subdirectory(common)
add_subdirectory(aes)
add_subdirectory(blowfish)
add_subdirectory(camellia)
add_subdirectory(cast128)
add_subdirectory(cast256)
add_subdirectory(des)
add_subdirectory(idea)
add_subdirectory(mysty1)
add_subdirectory(noekeon)
add_subdirectory(rc2)
add_subdirectory(rc5)
add_subdirectory(rc6)
add_subdirectory(serpent)
add_subdirectory(skipjack)
add_subdirectory(twofish)
add_subdirectory(xtea)


# # Set include directories for the interface library
# target_include_directories(BlockCipher INTERFACE 
#     ${CMAKE_CURRENT_SOURCE_DIR}
# )

# target_link_libraries(BlockCipher INTERFACE
#     cryptoGL::Core
#     cryptoGL::BlockCipherCommon
#     cryptoGL::AES
#     cryptoGL::Blowfish
#     cryptoGL::Camellia
#     cryptoGL::Cast128
#     cryptoGL::Cast256
#     cryptoGL::DES
#     cryptoGL::Idea
#     cryptoGL::Mysty1
#     cryptoGL::Noekeon
#     cryptoGL::RC2
#     cryptoGL::RC5
#     cryptoGL::RC6
#     cryptoGL::Serpent
#     cryptoGL::Skipjack
#     cryptoGL::Twofish
#     cryptoGL::Xtea
# )