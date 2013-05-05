cmake_minimum_required (VERSION 2.8)

set (Project_Name Cryptology)

project (${Project_Name})

#set (CMAKE_CXX_COMPILER             "/usr/clang_3_3/bin/clang++")
set (CMAKE_CXX_FLAGS                "-Wall -Werror -pedantic-errors -lgtest -pthread -std=c++11")

# Add the source files code (CPP, HPP and the CMakeLists.txt).
add_subdirectory (src)

#Add the testing source files.
add_subdirectory (testing)

enable_testing()
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

# Target the project name to be built.
add_executable(${Project_Name} src/main.cpp)

# Link the gtest librairies.
target_link_libraries(${Project_Name} ${GTEST_BOTH_LIBRARIES})

# Link the SRC librairies of the project.
target_link_libraries (${Project_Name} componants)

# Link the testing test classes librairies of the project.
target_link_libraries (${Project_Name} test_classes)

add_test(AllTestsInCryptology ${Project_Name})

