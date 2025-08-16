add_library(Camellia STATIC
    Camellia.cpp
    Camellia.hpp
)
add_library(cryptoGL::Camellia ALIAS Camellia)

target_include_directories(Camellia PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Camellia PUBLIC
    cryptoGL::Core
    cryptoGL::BlockCipherCommon
)

apply_crypto_security_flags(Camellia)
apply_performance_flags(Camellia)