# Create libcomponants.a with all the source files provided by SRC.
# add_library(Utils STATIC)
# add_library(cryptoGL::Utils ALIAS Utils)

# # Add source files
# target_sources(Utils PRIVATE
#     MathematicalTools.cpp
#     Padding.cpp
#     SquareMatrix.cpp
#     Tools.cpp
# )

# # Indiquer à CMake où trouver les headers pour les projets qui utilisent cette lib
# target_include_directories(Utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# # Apply security flags
# apply_crypto_security_flags(Utils)

# # Apply performance flags
#apply_performance_flags(Utils)