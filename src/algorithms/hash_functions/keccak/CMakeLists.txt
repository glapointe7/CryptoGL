add_library(Keccak STATIC
    Keccak.cpp
    Keccak.hpp
)
add_library(cryptoGL::Keccak ALIAS Keccak)

target_include_directories(Keccak PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Keccak PUBLIC
    cryptoGL::Core
    cryptoGL::HashFunctionsCommon
)

apply_crypto_security_flags(Keccak)
apply_performance_flags(Keccak)