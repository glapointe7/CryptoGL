add_library(RC2 STATIC
    RC2.cpp
    RC2.hpp
)
add_library(cryptoGL::RC2 ALIAS RC2)

target_include_directories(RC2 PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(RC2 PUBLIC
    cryptoGL::Core
    cryptoGL::BlockCipherCommon
)

apply_crypto_security_flags(RC2)
apply_performance_flags(RC2)