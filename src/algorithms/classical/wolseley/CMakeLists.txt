add_library(Wolseley STATIC
    Wolseley.cpp
    Wolseley.hpp
)
add_library(cryptoGL::Wolseley ALIAS Wolseley)

target_include_directories(Wolseley PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Wolseley PUBLIC
    cryptoGL::Core
    cryptoGL::ClassicalCommon
)

apply_crypto_security_flags(Wolseley)
apply_performance_flags(Wolseley)