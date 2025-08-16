add_library(Scream STATIC
    Scream.cpp
    Scream.hpp
)
add_library(cryptoGL::Scream ALIAS Scream)

target_include_directories(Scream PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Scream PUBLIC
    cryptoGL::Core
    cryptoGL::StreamCipherCommon
)

apply_crypto_security_flags(Scream)
apply_performance_flags(Scream)