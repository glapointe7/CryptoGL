cmake_minimum_required(VERSION 3.8)

# Project name and language
project(CryptoGL VERSION 1.0 LANGUAGES CXX)

# Optional: specify compiler (only if needed)
set(CMAKE_CXX_COMPILER "/usr/bin/g++-11")

# Use C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Common compile options
add_compile_options(-Wall -Wextra -Wpedantic -Werror -pthread)

# Add the source library directory
add_subdirectory(src)

# Include headers for unit tests (if needed)
include_directories(UnitTests)

# Define the main executable
add_executable(${PROJECT_NAME} src/main.cpp)

# Link internal libraries to the main executable
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    cryptoGL
    big_integers
)

# Optional: add test library if needed later
# target_link_libraries(${PROJECT_NAME} CryptoGL_UnitTests)
# enable_testing()
# add_test(NAME CryptoGL_UnitTests COMMAND ${PROJECT_NAME})