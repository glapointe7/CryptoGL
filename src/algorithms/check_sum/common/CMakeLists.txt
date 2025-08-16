# Create a STATIC library for common code
add_library(CheckSumCommon STATIC
    Checksum.cpp
    Checksum.hpp
)
add_library(cryptoGL::CheckSumCommon ALIAS CheckSumCommon)

# Set include directories for the common library
target_include_directories(CheckSumCommon PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}               
)

# Link common library to Core dependencies
target_link_libraries(CheckSumCommon PUBLIC
    cryptoGL::Core
)

# Apply security and performance flags to common library
apply_crypto_security_flags(CheckSumCommon)
apply_performance_flags(CheckSumCommon)