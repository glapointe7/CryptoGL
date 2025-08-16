add_library(CAST256 STATIC
    CAST256.cpp
    CAST256.hpp
)
add_library(cryptoGL::CAST256 ALIAS CAST256)

target_include_directories(CAST256 PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(CAST256 PUBLIC
    cryptoGL::Core
    cryptoGL::BlockCipherCommon
)

apply_crypto_security_flags(CAST256)
apply_performance_flags(CAST256)