#add_library(CheckSum INTERFACE)
#add_library(cryptoGL::CheckSum ALIAS CheckSum)
add_subdirectory(common)
add_subdirectory(adler32)
add_subdirectory(lrc)

# Set include directories for the interface library
# target_include_directories(CheckSum INTERFACE 
#     ${CMAKE_CURRENT_SOURCE_DIR}
#     ${CMAKE_CURRENT_SOURCE_DIR}/common
# )

# target_link_libraries(CheckSum INTERFACE
#     cryptoGL::Core
#     cryptoGL::CheckSumCommon
#     cryptoGL::Adler32
#     cryptoGL::LRC
# )