add_library(Blowfish STATIC
    Blowfish.cpp
    Blowfish.hpp
)
add_library(cryptoGL::Blowfish ALIAS Blowfish)

target_include_directories(Blowfish PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Blowfish PUBLIC
    cryptoGL::Core
    cryptoGL::BlockCipherCommon
)

apply_crypto_security_flags(Blowfish)
apply_performance_flags(Blowfish)