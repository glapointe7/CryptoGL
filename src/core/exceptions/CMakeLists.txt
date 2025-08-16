# Create library interface meaning no source code CPP in the folder, only header files (HPP).
# add_library(Exceptions INTERFACE)
# add_library(cryptoGL::Exceptions ALIAS Exceptions)

# target_include_directories(Exceptions INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Apply security flags
#apply_crypto_security_flags(Exceptions)

# Apply performance flags
#apply_performance_flags(Exceptions)