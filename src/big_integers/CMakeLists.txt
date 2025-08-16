# Get all .cpp files in the directory 'big_integers' and its subdirectories.
file(GLOB SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cc
)

# Create libBigIntegers.a with all the source files provided in SRC.
add_library(BigIntegers STATIC ${SRC})
add_library(cryptoGL::BigIntegers ALIAS BigIntegers)

# Allow in the code to include any HPP files from anywhere.
# For example, it allows to #include "big_integers/BigIntegers.hh" from any file.
target_include_directories(BigIntegers PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# This library has no dependencies to other libraries.
# Therefore, we do not need to use target_link_libraries.

# Apply security flags
apply_crypto_security_flags(BigIntegers)

# Apply performance flags
apply_performance_flags(BigIntegers)