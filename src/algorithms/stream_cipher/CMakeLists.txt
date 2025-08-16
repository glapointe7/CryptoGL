#add_library(StreamCipher INTERFACE)
#add_library(cryptoGL::StreamCipher ALIAS StreamCipher)
add_subdirectory(common)
add_subdirectory(hc256)
add_subdirectory(rabbit)
add_subdirectory(rc4)
add_subdirectory(salsa20)
add_subdirectory(scream)
add_subdirectory(seal)
add_subdirectory(snow3g)

# Set include directories for the interface library
# target_include_directories(StreamCipher INTERFACE 
#     ${CMAKE_CURRENT_SOURCE_DIR}
#     ${CMAKE_CURRENT_SOURCE_DIR}/common
# )

# target_link_libraries(StreamCipher INTERFACE
#     cryptoGL::Core
#     cryptoGL::StreamCipherCommon
#     cryptoGL::HC256
#     cryptoGL::Rabbit
#     cryptoGL::RC4
#     cryptoGL::Scream
#     cryptoGL::SEAL
#     cryptoGL::Snow3G
# )