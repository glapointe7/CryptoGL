add_library(Polybius STATIC
    Polybius.cpp
    Polybius.hpp
)
add_library(cryptoGL::Polybius ALIAS Polybius)

target_include_directories(Polybius PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Polybius PUBLIC
    cryptoGL::Core
    cryptoGL::ClassicalCommon
)

apply_crypto_security_flags(Polybius)
apply_performance_flags(Polybius)