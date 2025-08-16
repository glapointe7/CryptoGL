add_library(RC5 STATIC
    RC5.cpp
    RC5.hpp
)
add_library(cryptoGL::RC5 ALIAS RC5)

target_include_directories(RC5 PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(RC5 PUBLIC
    cryptoGL::Core
    cryptoGL::BlockCipherCommon
)

apply_crypto_security_flags(RC5)
apply_performance_flags(RC5)