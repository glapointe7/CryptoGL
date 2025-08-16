add_library(Noekeon STATIC
    Noekeon.cpp
    Noekeon.hpp
)
add_library(cryptoGL::Noekeon ALIAS Noekeon)

target_include_directories(Noekeon PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Noekeon PUBLIC
    cryptoGL::Core
    cryptoGL::BlockCipherCommon
)

apply_crypto_security_flags(Noekeon)
apply_performance_flags(Noekeon)