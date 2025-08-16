add_library(Affine STATIC
    Affine.cpp
    Affine.hpp
)
add_library(cryptoGL::Affine ALIAS Affine)

target_include_directories(Affine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Affine PUBLIC
    cryptoGL::Core
    cryptoGL::ClassicalCommon
)

apply_crypto_security_flags(Affine)
apply_performance_flags(Affine)