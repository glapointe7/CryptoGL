add_library(Salsa20 STATIC
    Salsa20.cpp
    Salsa20.hpp
)
add_library(cryptoGL::Salsa20 ALIAS Salsa20)

target_include_directories(Salsa20 PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Salsa20 PUBLIC
    cryptoGL::Core
    cryptoGL::HashFunctionsCommon
)

apply_crypto_security_flags(Salsa20)
apply_performance_flags(Salsa20)