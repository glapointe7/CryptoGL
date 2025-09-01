# Activate CTest given by CMake
enable_testing()

# Create the executable
add_executable(cryptoGL_tests
    TestsRun.cpp
)

target_include_directories(cryptoGL_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/unit
)

target_link_libraries(cryptoGL_tests PRIVATE
    gtest
    gtest_main
    cryptoGL::Core
    #cryptoGL::BigIntegers
    cryptoGL::AlgorithmsCommon
    
    # Block cipher
    cryptoGL::BlockCipherCommon
    cryptoGL::AES
    cryptoGL::Blowfish
    cryptoGL::Camellia
    cryptoGL::CAST128
    cryptoGL::CAST256
    cryptoGL::DES
    cryptoGL::IDEA
    cryptoGL::Mysty1
    cryptoGL::Noekeon
    cryptoGL::RC2
    cryptoGL::RC5
    cryptoGL::RC6
    cryptoGL::Serpent
    cryptoGL::Skipjack
    cryptoGL::Twofish
    cryptoGL::XTEA

    # Checksum
    cryptoGL::CheckSumCommon
    cryptoGL::Adler32
    cryptoGL::LRC

    # Classical
    cryptoGL::ClassicalCommon
    cryptoGL::Adfgvx
    cryptoGL::Affine
    cryptoGL::Chao
    cryptoGL::Collon
    cryptoGL::Delastelle
    cryptoGL::Fleissner
    cryptoGL::FourSquares
    cryptoGL::Hill
    cryptoGL::MonoalphabeticSubstitution
    cryptoGL::Morse
    cryptoGL::Nihilistes
    cryptoGL::Playfair
    cryptoGL::Polybius
    cryptoGL::Railfence
    cryptoGL::ThreeSquares
    cryptoGL::Transposition
    cryptoGL::TwoSquares
    cryptoGL::UnsortAlpha
    cryptoGL::Vigenere
    cryptoGL::Wolseley

    # Hash Functions
    cryptoGL::HashFunctionsCommon
    cryptoGL::Blake
    cryptoGL::Keccak
    cryptoGL::MD2
    cryptoGL::MessageDigest
    cryptoGL::Ripemd
    cryptoGL::Salsa20
    cryptoGL::SHA1
    cryptoGL::SHA2
    cryptoGL::Tiger
    cryptoGL::Whirlpool

    # Pseudo Random Functions
    cryptoGL::PseudoRandomFunctionCommon
    cryptoGL::Isaac

    # Public Key
    cryptoGL::PublicKeyCommon
    cryptoGL::HellmanMerkleKnapsack
    cryptoGL::RSA

    # Stream Cipher
    cryptoGL::StreamCipherCommon
    cryptoGL::HC256
    cryptoGL::Rabbit
    cryptoGL::RC4
    cryptoGL::Scream
    cryptoGL::SEAL
    cryptoGL::Snow3G
)

apply_crypto_security_flags(cryptoGL_tests)
apply_performance_flags(cryptoGL_tests)

add_test(NAME cryptoGL_tests COMMAND cryptoGL_tests)

set_tests_properties(cryptoGL_tests PROPERTIES
    TIMEOUT 120
    LABELS "unit"
)

# Add test subdirectories in logical order
add_subdirectory(unit)            # Individual algorithm tests
#add_subdirectory(integration)    # Integration tests
#add_subdirectory(security)       # Security-focused tests
#add_subdirectory(benchmarking)   # Performance benchmarks
#add_subdirectory(stress)         # Stress testing)

# # Apply test-specific compiler flags
# target_compile_options(cryptoGL_tests PRIVATE
#     $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-fprofile-arcs -ftest-coverage>
#     $<$<CXX_COMPILER_ID:MSVC>:/coverage>
# )