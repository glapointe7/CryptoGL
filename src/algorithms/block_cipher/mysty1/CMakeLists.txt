add_library(Mysty1 STATIC
    Mysty1.cpp
    Mysty1.hpp
)
add_library(cryptoGL::Mysty1 ALIAS Mysty1)

target_include_directories(Mysty1 PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Mysty1 PUBLIC
    cryptoGL::Core
    cryptoGL::BlockCipherCommon
)

apply_crypto_security_flags(Mysty1)
apply_performance_flags(Mysty1)