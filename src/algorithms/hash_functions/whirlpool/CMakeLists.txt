add_library(Whirlpool STATIC
    Whirlpool.cpp
    Whirlpool.hpp
)
add_library(cryptoGL::Whirlpool ALIAS Whirlpool)

target_include_directories(Whirlpool PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Whirlpool PUBLIC
    cryptoGL::Core
    cryptoGL::HashFunctionsCommon
)

apply_crypto_security_flags(Whirlpool)
apply_performance_flags(Whirlpool)