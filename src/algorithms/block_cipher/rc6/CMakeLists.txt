add_library(RC6 STATIC
    RC6.cpp
    RC6.hpp
)
add_library(cryptoGL::RC6 ALIAS RC6)

target_include_directories(RC6 PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(RC6 PUBLIC
    cryptoGL::Core
    cryptoGL::BlockCipherCommon
)

apply_crypto_security_flags(RC6)
apply_performance_flags(RC6)