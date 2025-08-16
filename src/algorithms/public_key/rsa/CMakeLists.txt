add_library(RSA STATIC
    RSA.cpp
    RSA.hpp
)
add_library(cryptoGL::RSA ALIAS RSA)

target_include_directories(RSA PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(RSA PUBLIC
    cryptoGL::Core
    cryptoGL::BigIntegers
    cryptoGL::PublicKeyCommon
)

apply_crypto_security_flags(RSA)
apply_performance_flags(RSA)