#add_library(PseudoRandomGenerator INTERFACE)
#add_library(cryptoGL::PseudoRandomGenerator ALIAS PseudoRandomGenerator)
add_subdirectory(common)
add_subdirectory(isaac)

# Set include directories for the interface library
# target_include_directories(PseudoRandomGenerator INTERFACE 
#     ${CMAKE_CURRENT_SOURCE_DIR}
#     ${CMAKE_CURRENT_SOURCE_DIR}/common
# )

# target_link_libraries(PseudoRandomGenerator INTERFACE
#     cryptoGL::Core
#     cryptoGL::PseudoRandomGeneratorCommon
#     cryptoGL::Isaac
# )