cmake_minimum_required (VERSION 2.8)

set (Project_Name CryptoGL)

project (${Project_Name})

set (CMAKE_CXX_COMPILER             "/usr/bin/clang++-3.6")
set (CMAKE_CXX_FLAGS                "-Winline -Wall -Werror -pedantic-errors -pthread -std=c++1z")

# Add the source files code (CPP, HPP and the CMakeLists.txt).
add_subdirectory (src)

#Include the unit test source file headers.
#enable_testing()
include_directories (UnitTests)

# Target the project name to be built.
add_executable(${Project_Name} src/main.cpp)

# Link the src librairy to the project.
target_link_libraries (${Project_Name} cryptoGL)

# Link the big_integers librairy to the project.
target_link_libraries (${Project_Name} big_integers)

# Link the UnitTests librairy to the project.
#target_link_libraries (${Project_Name} CryptoGL_UnitTests)

#add_test(CryptoGL_UnitTests ${Project_Name})

