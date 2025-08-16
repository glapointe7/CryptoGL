add_library(Morse STATIC
    Morse.cpp
    Morse.hpp
)
add_library(cryptoGL::Morse ALIAS Morse)

target_include_directories(Morse PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Morse PUBLIC
    cryptoGL::Core
    cryptoGL::ClassicalCommon
)

apply_crypto_security_flags(Morse)
apply_performance_flags(Morse)