# add_library(Types STATIC)
# add_library(cryptoGL::Types ALIAS Types)

# Add source files
# target_sources(Types PRIVATE
#     String.cpp
#     String.cpp
#     Integer.hpp
#     Types.hpp
#     Vector.hpp
# )

# Indiquer à CMake où trouver les headers pour les projets qui utilisent cette lib
#target_include_directories(Types PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Apply security flags
#apply_crypto_security_flags(Types)

# Apply performance flags
#apply_performance_flags(Types)