add_library(TwoSquares STATIC
    TwoSquares.cpp
    TwoSquares.hpp
)
add_library(cryptoGL::TwoSquares ALIAS TwoSquares)

target_include_directories(TwoSquares PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(TwoSquares PUBLIC
    cryptoGL::Core
    cryptoGL::ClassicalCommon
)

apply_crypto_security_flags(TwoSquares)
apply_performance_flags(TwoSquares)