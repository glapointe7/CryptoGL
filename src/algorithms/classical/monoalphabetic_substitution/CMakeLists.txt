add_library(MonoalphabeticSubstitution STATIC
    MonoalphabeticSubstitution.cpp
    MonoalphabeticSubstitution.hpp
)
add_library(cryptoGL::MonoalphabeticSubstitution ALIAS MonoalphabeticSubstitution)

target_include_directories(MonoalphabeticSubstitution PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(MonoalphabeticSubstitution PUBLIC
    cryptoGL::Core
    cryptoGL::ClassicalCommon
)

apply_crypto_security_flags(MonoalphabeticSubstitution)
apply_performance_flags(MonoalphabeticSubstitution)