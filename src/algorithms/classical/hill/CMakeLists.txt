add_library(Hill STATIC
    Hill.cpp
    Hill.hpp
)
add_library(cryptoGL::Hill ALIAS Hill)

target_include_directories(Hill PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Hill PUBLIC
    cryptoGL::Core
    cryptoGL::ClassicalCommon
)

apply_crypto_security_flags(Hill)
apply_performance_flags(Hill)