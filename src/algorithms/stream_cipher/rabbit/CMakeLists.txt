add_library(Rabbit STATIC
    Rabbit.cpp
    Rabbit.hpp
)
add_library(cryptoGL::Rabbit ALIAS Rabbit)

target_include_directories(Rabbit PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Rabbit PUBLIC
    cryptoGL::Core
    cryptoGL::StreamCipherCommon
)

apply_crypto_security_flags(Rabbit)
apply_performance_flags(Rabbit)