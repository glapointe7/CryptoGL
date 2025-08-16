add_library(AES STATIC
    AES.cpp
    AES.hpp
)
add_library(cryptoGL::AES ALIAS AES)

target_include_directories(AES PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(AES PUBLIC
    cryptoGL::Core
    cryptoGL::BlockCipherCommon
)

apply_crypto_security_flags(AES)
apply_performance_flags(AES)