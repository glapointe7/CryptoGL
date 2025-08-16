add_library(Playfair STATIC
    Playfair.cpp
    Playfair.hpp
)
add_library(cryptoGL::Playfair ALIAS Playfair)

target_include_directories(Playfair PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Playfair PUBLIC
    cryptoGL::Core
    cryptoGL::ClassicalCommon
)

apply_crypto_security_flags(Playfair)
apply_performance_flags(Playfair)