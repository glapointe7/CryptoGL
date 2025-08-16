add_library(HC256 STATIC
    HC256.cpp
    HC256.hpp
)
add_library(cryptoGL::HC256 ALIAS HC256)

target_include_directories(HC256 PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(HC256 PUBLIC
    cryptoGL::Core
    cryptoGL::StreamCipherCommon
)

apply_crypto_security_flags(HC256)
apply_performance_flags(HC256)