cmake_minimum_required (VERSION 2.8)

set (Project_Name Cryptology)

project (${Project_Name})

#set (CMAKE_CXX_COMPILER             "/usr/clang_3_3/bin/clang++")
set (CMAKE_CXX_FLAGS                "-Winline -Wall -Werror -pedantic-errors -lgtest -pthread -std=c++11")

# Add the source files code (CPP, HPP and the CMakeLists.txt).
add_subdirectory (src)

#Include the testing source file headers.
include_directories (testing)

enable_testing()
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

# Target the project name to be built.
add_executable(${Project_Name} src/main.cpp)

# Link the gtest librairies.
target_link_libraries(${Project_Name} ${GTEST_BOTH_LIBRARIES})

# Link the SRC librairies of the project.
target_link_libraries (${Project_Name} componants)

add_test(AllTestsInCryptology ${Project_Name})

