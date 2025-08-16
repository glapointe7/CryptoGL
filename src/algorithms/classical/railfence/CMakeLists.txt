add_library(Railfence STATIC
    Railfence.cpp
    Railfence.hpp
)
add_library(cryptoGL::Railfence ALIAS Railfence)

target_include_directories(Railfence PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Railfence PUBLIC
    cryptoGL::Core
    cryptoGL::ClassicalCommon
)

apply_crypto_security_flags(Railfence)
apply_performance_flags(Railfence)