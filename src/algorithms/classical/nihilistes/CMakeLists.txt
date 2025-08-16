add_library(Nihilistes STATIC
    Nihilistes.cpp
    Nihilistes.hpp
)
add_library(cryptoGL::Nihilistes ALIAS Nihilistes)

target_include_directories(Nihilistes PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Nihilistes PUBLIC
    cryptoGL::Core
    cryptoGL::ClassicalCommon
)

apply_crypto_security_flags(Nihilistes)
apply_performance_flags(Nihilistes)