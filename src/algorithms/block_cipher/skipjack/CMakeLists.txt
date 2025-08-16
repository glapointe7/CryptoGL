add_library(Skipjack STATIC
    Skipjack.cpp
    Skipjack.hpp
)
add_library(cryptoGL::Skipjack ALIAS Skipjack)

target_include_directories(Skipjack PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Skipjack PUBLIC
    cryptoGL::Core
    cryptoGL::BlockCipherCommon
)

apply_crypto_security_flags(Skipjack)
apply_performance_flags(Skipjack)