# Create a STATIC library for common code
add_library(PseudoRandomFunctionCommon STATIC
    PseudoRandomFunction.cpp
    PseudoRandomFunction.hpp
)
add_library(cryptoGL::PseudoRandomFunctionCommon ALIAS PseudoRandomFunctionCommon)

# Set include directories for the common library
target_include_directories(PseudoRandomFunctionCommon PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}               
)

# Link common library to Core dependencies
target_link_libraries(PseudoRandomFunctionCommon PUBLIC
    cryptoGL::Core
)

# Apply security and performance flags to common library
apply_crypto_security_flags(PseudoRandomFunctionCommon)
apply_performance_flags(PseudoRandomFunctionCommon)