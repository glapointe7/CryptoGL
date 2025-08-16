add_library(Twofish STATIC
    Twofish.cpp
    Twofish.hpp
)
add_library(cryptoGL::Twofish ALIAS Twofish)

target_include_directories(Twofish PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Twofish PUBLIC
    cryptoGL::Core
    cryptoGL::BlockCipherCommon
)

apply_crypto_security_flags(Twofish)
apply_performance_flags(Twofish)