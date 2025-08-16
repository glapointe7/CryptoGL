# Create a STATIC library for common code
add_library(BlockCipherCommon STATIC
    BlockCipherModes.cpp
    BlockCipherModes.hpp
    BlockCipher.hpp
    Feistel.hpp
    MAC.hpp
)
add_library(cryptoGL::BlockCipherCommon ALIAS BlockCipherCommon)

# Set include directories for the common library
target_include_directories(BlockCipherCommon PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}               
)

# Link common library to Core dependencies
target_link_libraries(BlockCipherCommon PUBLIC
    cryptoGL::Core
)

# Apply security and performance flags to common library
apply_crypto_security_flags(BlockCipherCommon)
apply_performance_flags(BlockCipherCommon)