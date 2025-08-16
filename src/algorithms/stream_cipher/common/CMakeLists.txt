# Create a STATIC library for common code
add_library(StreamCipherCommon STATIC
    StreamCipher.hpp
    SynchronousStreamCipher.hpp
)
add_library(cryptoGL::StreamCipherCommon ALIAS StreamCipherCommon)

# Set include directories for the common library
target_include_directories(StreamCipherCommon PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}               
)

# Link common library to Core dependencies
target_link_libraries(StreamCipherCommon PUBLIC
    cryptoGL::Core
)

# Apply security and performance flags to common library
apply_crypto_security_flags(StreamCipherCommon)
apply_performance_flags(StreamCipherCommon)