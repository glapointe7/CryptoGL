add_library(XTEA STATIC
    XTEA.cpp
    XTEA.hpp
)
add_library(cryptoGL::XTEA ALIAS XTEA)

target_include_directories(XTEA PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(XTEA PUBLIC
    cryptoGL::Core
    cryptoGL::BlockCipherCommon
)

apply_crypto_security_flags(XTEA)
apply_performance_flags(XTEA)