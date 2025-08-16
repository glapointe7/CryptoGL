add_library(MessageDigest STATIC
    MessageDigest.cpp
    MessageDigest.hpp
)
add_library(cryptoGL::MessageDigest ALIAS MessageDigest)

target_include_directories(MessageDigest PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(MessageDigest PUBLIC
    cryptoGL::Core
    cryptoGL::HashFunctionsCommon
)

apply_crypto_security_flags(MessageDigest)
apply_performance_flags(MessageDigest)