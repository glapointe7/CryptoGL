add_library(Tiger STATIC
    Tiger.cpp
    Tiger.hpp
)
add_library(cryptoGL::Tiger ALIAS Tiger)

target_include_directories(Tiger PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Tiger PUBLIC
    cryptoGL::Core
    cryptoGL::HashFunctionsCommon
)

apply_crypto_security_flags(Tiger)
apply_performance_flags(Tiger)