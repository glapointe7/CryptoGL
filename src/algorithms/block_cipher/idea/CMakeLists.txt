add_library(IDEA STATIC
    IDEA.cpp
    IDEA.hpp
)
add_library(cryptoGL::IDEA ALIAS IDEA)

target_include_directories(IDEA PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(IDEA PUBLIC
    cryptoGL::Core
    cryptoGL::BlockCipherCommon
)

apply_crypto_security_flags(IDEA)
apply_performance_flags(IDEA)