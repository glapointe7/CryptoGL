#add_library(PublicKey INTERFACE)
#add_library(cryptoGL::PublicKey ALIAS PublicKey)
add_subdirectory(common)
add_subdirectory(rsa)
add_subdirectory(hellman_merkle)

# Set include directories for the interface library
# target_include_directories(PublicKey INTERFACE 
#     ${CMAKE_CURRENT_SOURCE_DIR}
#     ${CMAKE_CURRENT_SOURCE_DIR}/common
# )

# target_link_libraries(PublicKey INTERFACE
#     cryptoGL::Core
#     cryptoGL::PublicKeyCommon
#     cryptoGL::HellmanMerkle
#     cryptoGL::RSA
# )