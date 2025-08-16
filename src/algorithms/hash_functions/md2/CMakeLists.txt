add_library(MD2 STATIC
    MD2.cpp
    MD2.hpp
)
add_library(cryptoGL::MD2 ALIAS MD2)

target_include_directories(MD2 PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(MD2 PUBLIC
    cryptoGL::Core
    cryptoGL::HashFunctionsCommon
)

apply_crypto_security_flags(MD2)
apply_performance_flags(MD2)