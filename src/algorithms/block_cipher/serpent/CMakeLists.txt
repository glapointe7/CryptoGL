add_library(Serpent STATIC
    Serpent.cpp
    Serpent.hpp
)
add_library(cryptoGL::Serpent ALIAS Serpent)

target_include_directories(Serpent PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Serpent PUBLIC
    cryptoGL::Core
    cryptoGL::BlockCipherCommon
)

apply_crypto_security_flags(Serpent)
apply_performance_flags(Serpent)