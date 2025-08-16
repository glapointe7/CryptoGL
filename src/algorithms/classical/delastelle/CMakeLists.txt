add_library(Delastelle STATIC
    Delastelle.cpp
    Delastelle.hpp
)
add_library(cryptoGL::Delastelle ALIAS Delastelle)

target_include_directories(Delastelle PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Delastelle PUBLIC
    cryptoGL::Core
    cryptoGL::ClassicalCommon
)

apply_crypto_security_flags(Delastelle)
apply_performance_flags(Delastelle)