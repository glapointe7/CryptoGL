add_library(UnsortAlpha STATIC
    UnsortAlpha.cpp
    UnsortAlpha.hpp
)
add_library(cryptoGL::UnsortAlpha ALIAS UnsortAlpha)

target_include_directories(UnsortAlpha PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(UnsortAlpha PUBLIC
    cryptoGL::Core
    cryptoGL::ClassicalCommon
)

apply_crypto_security_flags(UnsortAlpha)
apply_performance_flags(UnsortAlpha)