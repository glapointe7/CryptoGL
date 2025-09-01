# Add the source files code (CPP, HPP and the CMakeLists.txt) of 'big_numbers' directory.
add_subdirectory(core)
add_subdirectory(algorithms)
#add_subdirectory(big_integers)

# Create an executable for real usages of the algorithms.
add_executable(cryptoGL
    main.cpp
)

# The executable is dependent of Core, Algorithms and BigIntegers libraries.
# Therefore, these libraries must be linked with the executable.
target_link_libraries(cryptoGL PRIVATE
    cryptoGL::Core
    #cryptoGL::BigIntegers
    cryptoGL::AlgorithmsCommon
    
    # Block cipher
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
    cryptoGL::Adler32
    cryptoGL::LRC

    # Classical
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
    cryptoGL::Isaac

    # Public Key
    cryptoGL::HellmanMerkleKnapsack
    cryptoGL::RSA

    # Stream Cipher
    cryptoGL::HC256
    cryptoGL::Rabbit
    cryptoGL::RC4
    cryptoGL::Scream
    cryptoGL::SEAL
    cryptoGL::Snow3G
)

# Apply security flags to executable
apply_crypto_security_flags(cryptoGL)

# Apply performance flags to executable
apply_performance_flags(cryptoGL)