add_library(ThreeSquares STATIC
    ThreeSquares.cpp
    ThreeSquares.hpp
)
add_library(cryptoGL::ThreeSquares ALIAS ThreeSquares)

target_include_directories(ThreeSquares PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ThreeSquares PUBLIC
    cryptoGL::Core
    cryptoGL::ClassicalCommon
)

apply_crypto_security_flags(ThreeSquares)
apply_performance_flags(ThreeSquares)