# Create a STATIC library for common code
add_library(PublicKeyCommon STATIC
    AsymmetricCipher.cpp
    AsymmetricCipher.hpp
)
add_library(cryptoGL::PublicKeyCommon ALIAS PublicKeyCommon)

# Set include directories for the common library
target_include_directories(PublicKeyCommon PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}               
)

# Link common library to Core dependencies
target_link_libraries(PublicKeyCommon PUBLIC
    cryptoGL::Core
    cryptoGL::BigIntegers
)

# Apply security and performance flags to common library
apply_crypto_security_flags(PublicKeyCommon)
apply_performance_flags(PublicKeyCommon)