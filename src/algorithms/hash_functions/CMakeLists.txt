#add_library(HashFunctions INTERFACE)
#add_library(cryptoGL::HashFunctions ALIAS HashFunctions)
add_subdirectory(common)
add_subdirectory(blake)
add_subdirectory(keccak)
add_subdirectory(md2)
add_subdirectory(message_digest)
add_subdirectory(ripemd)
add_subdirectory(sha1)
add_subdirectory(sha2)
add_subdirectory(tiger)
add_subdirectory(whirlpool)

# Set include directories for the interface library
# target_include_directories(HashFunctions INTERFACE 
#     ${CMAKE_CURRENT_SOURCE_DIR}
#     ${CMAKE_CURRENT_SOURCE_DIR}/common
# )

# target_link_libraries(HashFunctions INTERFACE
#     cryptoGL::Core
#     cryptoGL::HashFunctionsCommon
#     cryptoGL::Blake
#     cryptoGL::Keccak
#     cryptoGL::MD2
#     cryptoGL::MessageDigest
#     cryptoGL::Ripemd
#     cryptoGL::Salsa20
#     cryptoGL::SHA1
#     cryptoGL::SHA2
#     cryptoGL::Tiger
#     cryptoGL::Whirlpool
# )