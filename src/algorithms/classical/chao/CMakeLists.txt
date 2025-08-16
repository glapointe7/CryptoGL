add_library(Chao STATIC
    Chao.cpp
    Chao.hpp
)
add_library(cryptoGL::Chao ALIAS Chao)

target_include_directories(Chao PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Chao PUBLIC
    cryptoGL::Core
    cryptoGL::ClassicalCommon
)

apply_crypto_security_flags(Chao)
apply_performance_flags(Chao)