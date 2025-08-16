cmake_minimum_required(VERSION 3.27)

# Project name, version and language
project(CryptoGL VERSION 1.0 LANGUAGES CXX)

# Include custom CMake modules
include(cmake/CompilerFlags.cmake)
include(cmake/SecurityHardening.cmake)
include(cmake/PerformanceOptimization.cmake)
include(cmake/Testing.cmake)

# Use C++20 standard and compile with std=c++20 instead of std=g++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use absolute paths in target_sources
cmake_policy(SET CMP0077 NEW)

# Use absolute paths in file(GLOB)
cmake_policy(SET CMP0135 NEW)

# We use the GoogleTest framework for unit tests.
find_package(GTest REQUIRED)

# Add subdirectories in dependency order (this creates the library targets and alias 'cryptoGL::')
add_subdirectory(src)
add_subdirectory(tests)

# # Main executable with proper linking
# add_executable(${PROJECT_NAME} src/main.cpp)

# target_link_libraries(${PROJECT_NAME} PRIVATE
#     cryptoGL::Core
#     cryptoGL::Algorithms
#     cryptoGL::BigIntegers
# )

# Apply security flags to main executable
#apply_crypto_security_flags(${PROJECT_NAME})

# Apply performance flags to main executable
#apply_performance_flags(${PROJECT_NAME})

# target_include_directories(${PROJECT_NAME} PUBLIC
#     ${CMAKE_SOURCE_DIR}/src
# )

# # Tests
# enable_testing()
# add_subdirectory(tests)      # Main test suite with 5 subdirectories