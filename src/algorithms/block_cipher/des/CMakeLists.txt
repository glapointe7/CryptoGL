add_library(DES STATIC
    DES.cpp
    DES.hpp
    TripleDES.cpp
    TripleDES.hpp
)
add_library(cryptoGL::DES ALIAS DES)

target_include_directories(DES PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(DES PUBLIC
    cryptoGL::Core
    cryptoGL::BlockCipherCommon
)

apply_crypto_security_flags(DES)
apply_performance_flags(DES)