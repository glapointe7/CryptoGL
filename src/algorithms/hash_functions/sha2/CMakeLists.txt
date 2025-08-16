add_library(SHA2 STATIC
    SHA2.cpp
    SHA2.hpp
)
add_library(cryptoGL::SHA2 ALIAS SHA2)

target_include_directories(SHA2 PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(SHA2 PUBLIC
    cryptoGL::Core
    cryptoGL::HashFunctionsCommon
)

apply_crypto_security_flags(SHA2)
apply_performance_flags(SHA2)