add_library(LRC STATIC
    LRC.cpp
    LRC.hpp
)
add_library(cryptoGL::LRC ALIAS LRC)

target_include_directories(LRC PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(LRC PUBLIC
    cryptoGL::Core
    cryptoGL::CheckSumCommon
)

apply_crypto_security_flags(LRC)
apply_performance_flags(LRC)