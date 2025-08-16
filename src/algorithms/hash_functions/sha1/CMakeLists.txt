add_library(SHA1 STATIC
    SHA1.cpp
    SHA1.hpp
)
add_library(cryptoGL::SHA1 ALIAS SHA1)

target_include_directories(SHA1 PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(SHA1 PUBLIC
    cryptoGL::Core
    cryptoGL::HashFunctionsCommon
)

apply_crypto_security_flags(SHA1)
apply_performance_flags(SHA1)