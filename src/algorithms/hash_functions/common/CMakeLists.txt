# Create a STATIC library for common code
add_library(HashFunctionsCommon STATIC
    HashFunction.hpp
    MerkleDamgardFunction.hpp
    SpongeFunction.hpp
)
add_library(cryptoGL::HashFunctionsCommon ALIAS HashFunctionsCommon)

# Set include directories for the common library
target_include_directories(HashFunctionsCommon PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}               
)

# Link common library to Core dependencies
target_link_libraries(HashFunctionsCommon PUBLIC
    cryptoGL::Core
)

# Apply security and performance flags to common library
apply_crypto_security_flags(HashFunctionsCommon)
apply_performance_flags(HashFunctionsCommon)