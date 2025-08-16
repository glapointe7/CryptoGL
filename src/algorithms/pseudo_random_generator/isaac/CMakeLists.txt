add_library(Isaac STATIC
    Isaac.cpp
    Isaac.hpp
)
add_library(cryptoGL::Isaac ALIAS Isaac)

target_include_directories(Isaac PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Isaac PUBLIC
    cryptoGL::Core
    cryptoGL::PseudoRandomFunctionCommon
)

apply_crypto_security_flags(Isaac)
apply_performance_flags(Isaac)