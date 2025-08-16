add_library(SEAL STATIC
    SEAL.cpp
    SEAL.hpp
)
add_library(cryptoGL::SEAL ALIAS SEAL)

target_include_directories(SEAL PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(SEAL PUBLIC
    cryptoGL::Core
    cryptoGL::StreamCipherCommon
)

apply_crypto_security_flags(SEAL)
apply_performance_flags(SEAL)