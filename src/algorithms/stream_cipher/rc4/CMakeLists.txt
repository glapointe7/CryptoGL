add_library(RC4 STATIC
    RC4.cpp
    RC4.hpp
)
add_library(cryptoGL::RC4 ALIAS RC4)

target_include_directories(RC4 PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(RC4 PUBLIC
    cryptoGL::Core
    cryptoGL::StreamCipherCommon
)

apply_crypto_security_flags(RC4)
apply_performance_flags(RC4)