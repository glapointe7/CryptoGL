add_library(Ripemd STATIC
    Ripemd.cpp
    Ripemd.hpp
)
add_library(cryptoGL::Ripemd ALIAS Ripemd)

target_include_directories(Ripemd PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Ripemd PUBLIC
    cryptoGL::Core
    cryptoGL::HashFunctionsCommon
)

apply_crypto_security_flags(Ripemd)
apply_performance_flags(Ripemd)