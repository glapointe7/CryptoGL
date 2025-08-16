add_library(Vigenere STATIC
    Vigenere.cpp
    Vigenere.hpp
)
add_library(cryptoGL::Vigenere ALIAS Vigenere)

target_include_directories(Vigenere PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Vigenere PUBLIC
    cryptoGL::Core
    cryptoGL::ClassicalCommon
)

apply_crypto_security_flags(Vigenere)
apply_performance_flags(Vigenere)