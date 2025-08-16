add_library(HellmanMerkleKnapsack STATIC
    HellmanMerkleKnapsack.cpp
    HellmanMerkleKnapsack.hpp
)
add_library(cryptoGL::HellmanMerkleKnapsack ALIAS HellmanMerkleKnapsack)

target_include_directories(HellmanMerkleKnapsack PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(HellmanMerkleKnapsack PUBLIC
    cryptoGL::Core
    cryptoGL::PublicKeyCommon
)

apply_crypto_security_flags(HellmanMerkleKnapsack)
apply_performance_flags(HellmanMerkleKnapsack)