add_library(Adfgvx STATIC
    Adfgvx.cpp
    Adfgvx.hpp
)
add_library(cryptoGL::Adfgvx ALIAS Adfgvx)

target_include_directories(Adfgvx PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Adfgvx PUBLIC
    cryptoGL::Core
    cryptoGL::ClassicalCommon
)

apply_crypto_security_flags(Adfgvx)
apply_performance_flags(Adfgvx)