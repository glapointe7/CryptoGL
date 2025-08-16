add_library(Transposition STATIC
    Transposition.cpp
    Transposition.hpp
)
add_library(cryptoGL::Transposition ALIAS Transposition)

target_include_directories(Transposition PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Transposition PUBLIC
    cryptoGL::Core
    cryptoGL::ClassicalCommon
)

apply_crypto_security_flags(Transposition)
apply_performance_flags(Transposition)