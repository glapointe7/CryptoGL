add_library(CAST128 STATIC
    CAST128.cpp
    CAST128.hpp
)
add_library(cryptoGL::CAST128 ALIAS CAST128)

target_include_directories(CAST128 PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(CAST128 PUBLIC
    cryptoGL::Core
    cryptoGL::BlockCipherCommon
)

apply_crypto_security_flags(CAST128)
apply_performance_flags(CAST128)