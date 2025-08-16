add_library(Adler32 STATIC
    Adler32.cpp
    Adler32.hpp
)
add_library(cryptoGL::Adler32 ALIAS Adler32)

target_include_directories(Adler32 PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Adler32 PUBLIC
    cryptoGL::Core
    cryptoGL::CheckSumCommon
)

apply_crypto_security_flags(Adler32)
apply_performance_flags(Adler32)