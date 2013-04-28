cmake_minimum_required (VERSION 2.8)
project (Cryptology)

#set (CMAKE_CXX_COMPILER             "/usr/clang_3_3/bin/clang++")
set (CMAKE_CXX_FLAGS                "-Wall -pedantic-errors -std=c++11")

#enable_testing()

add_subdirectory(src)
#add_subdirectory(testing)
