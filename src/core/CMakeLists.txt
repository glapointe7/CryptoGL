# Get all .cpp files in the directory 'core' and its subdirectories.
file(GLOB_RECURSE SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

# Create libCore.a with all the source files provided in SRC.
add_library(Core STATIC ${SRC})
add_library(cryptoGL::Core ALIAS Core)

# Allow in the code to include any HPP files from anywhere.
# For example, it allows to #include "core/exceptions/BadKeyLength.hpp" from any file.
target_include_directories(Core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# This library has no dependencies to other libraries.
# Therefore, we do not need to use target_link_libraries.

# Add all needed subdirectories in the Core library.
add_subdirectory(exceptions)
add_subdirectory(types)
add_subdirectory(Utils)