# Create a STATIC library for common code
add_library(AlgorithmsCommon STATIC
    Base64.cpp
    Base64.hpp
    Cipher.hpp
    SymmetricCipher.hpp
)
add_library(cryptoGL::AlgorithmsCommon ALIAS AlgorithmsCommon)

# Set include directories for the common library
target_include_directories(AlgorithmsCommon PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}               
)

# Link common library to Core dependencies
target_link_libraries(AlgorithmsCommon PUBLIC
    cryptoGL::Core
)

# Apply security and performance flags to common library
apply_crypto_security_flags(AlgorithmsCommon)
apply_performance_flags(AlgorithmsCommon)