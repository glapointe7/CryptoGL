add_library(Fleissner STATIC
    Fleissner.cpp
    Fleissner.hpp
)
add_library(cryptoGL::Fleissner ALIAS Fleissner)

target_include_directories(Fleissner PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Fleissner PUBLIC
    cryptoGL::Core
    cryptoGL::ClassicalCommon
)

apply_crypto_security_flags(Fleissner)
apply_performance_flags(Fleissner)