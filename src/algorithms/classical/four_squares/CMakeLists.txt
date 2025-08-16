add_library(FourSquares STATIC
    FourSquares.cpp
    FourSquares.hpp
)
add_library(cryptoGL::FourSquares ALIAS FourSquares)

target_include_directories(FourSquares PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(FourSquares PUBLIC
    cryptoGL::Core
    cryptoGL::ClassicalCommon
)

apply_crypto_security_flags(FourSquares)
apply_performance_flags(FourSquares)