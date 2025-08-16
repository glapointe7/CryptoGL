# Create a STATIC library for common code
add_library(ClassicalCommon STATIC
    CipherGrid.cpp
    CipherGrid.hpp
    SquareCipher.hpp
    StringCipher.cpp
    StringCipher.hpp
    StringCipherWithPermutationKey.cpp
    StringCipherWithPermutationKey.hpp
    StringCipherWithStringKey.cpp
    StringCipherWithStringKey.hpp
)
add_library(cryptoGL::ClassicalCommon ALIAS ClassicalCommon)

# Set include directories for the common library
target_include_directories(ClassicalCommon PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}               
)

# Link common library to Core dependencies
target_link_libraries(ClassicalCommon PUBLIC
    cryptoGL::Core
)

# Apply security and performance flags to common library
apply_crypto_security_flags(ClassicalCommon)
apply_performance_flags(ClassicalCommon)