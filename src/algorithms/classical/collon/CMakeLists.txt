add_library(Collon STATIC
    Collon.cpp
    Collon.hpp
)
add_library(cryptoGL::Collon ALIAS Collon)

target_include_directories(Collon PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Collon PUBLIC
    cryptoGL::Core
    cryptoGL::ClassicalCommon
)

apply_crypto_security_flags(Collon)
apply_performance_flags(Collon)