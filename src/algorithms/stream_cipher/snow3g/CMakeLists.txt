add_library(Snow3G STATIC
    Snow3G.cpp
    Snow3G.hpp
)
add_library(cryptoGL::Snow3G ALIAS Snow3G)

target_include_directories(Snow3G PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Snow3G PUBLIC
    cryptoGL::Core
    cryptoGL::StreamCipherCommon
)

apply_crypto_security_flags(Snow3G)
apply_performance_flags(Snow3G)