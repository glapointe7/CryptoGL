add_library(Blake STATIC
    Blake.cpp
    Blake.hpp
)
add_library(cryptoGL::Blake ALIAS Blake)

target_include_directories(Blake PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Blake PUBLIC
    cryptoGL::Core
    cryptoGL::HashFunctionsCommon
)

apply_crypto_security_flags(Blake)
apply_performance_flags(Blake)