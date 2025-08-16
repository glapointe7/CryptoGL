# Bitslicing in Cryptography: Educational Materials

This directory contains comprehensive educational materials about bitslicing in cryptography, including mathematical theory, practical implementations, and complete C++17 code examples.

## Contents

### 1. Educational Document
- **`BITSLICING_CRYPTOGRAPHY_EDUCATIONAL_GUIDE.tex`** - Complete LaTeX document covering:
  - Mathematical foundations of bitslicing
  - Boolean function representation
  - Bitsliced implementations of Serpent, AES, and DES
  - Test vectors and validation
  - Performance analysis
  - Side-channel resistance considerations

### 2. Implementation Files
- **`src/BitslicingExamples.hpp`** - Header file with class declarations
- **`src/BitslicingExamples.cpp`** - Complete C++17 implementations
- **`src/bitslicing_demo.cpp`** - Demo program to run examples

## What is Bitslicing?

Bitslicing is a technique for implementing cryptographic algorithms by representing multiple blocks of data as a set of words, where each word contains one bit from each block. This transformation allows bit-oriented operations to be performed as word-oriented operations, enabling efficient parallel processing.

### Key Benefits:
- **Parallel Processing**: Process multiple blocks simultaneously
- **Constant-Time Operations**: Natural resistance against timing attacks
- **Efficient Implementation**: Better use of modern processor capabilities
- **Mathematical Clarity**: Clean, mathematical approach to implementation

## Mathematical Foundation

The mathematical basis of bitslicing lies in Boolean algebra and finite field theory:

1. **Boolean Function Representation**: Any S-box can be represented as a set of Boolean functions
2. **Algebraic Normal Form (ANF)**: Boolean functions can be expressed as XORs of ANDs
3. **Parallel Processing**: Multiple blocks can be processed with the same operations

## Algorithms Covered

### 1. Serpent
- 32 rounds with 8 different S-boxes
- 128-bit block size, 256-bit key size
- Complete bitsliced S-box implementations
- Linear transformation in bitsliced form

### 2. AES (Advanced Encryption Standard)
- 10 rounds for AES-128
- 128-bit block size
- Bitsliced S-box using multiplicative inverse in GF(2^8)
- SubBytes, ShiftRows, and MixColumns transformations

### 3. DES (Data Encryption Standard)
- 16 Feistel rounds
- 64-bit block size, 56-bit key size
- Bitsliced S-box implementations
- Feistel network in bitsliced form

## Building and Running

### Prerequisites
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.10 or higher
- LaTeX distribution (for building the document)

### Building the Document
```bash
cd doc
pdflatex BITSLICING_CRYPTOGRAPHY_EDUCATIONAL_GUIDE.tex
```

### Building the C++ Examples
```bash
# From the project root
mkdir build
cd build
cmake ..
make bitslicing_demo
```

### Running the Demo
```bash
./bitslicing_demo
```

## Code Examples

### Basic Bitslicing Conversion
```cpp
// Convert traditional blocks to bitsliced format
std::vector<std::array<uint8_t, 16>> blocks = {{plaintext}};
std::array<uint32_t, 128> bitsliced;
convert_to_bitsliced<128>(blocks, bitsliced);

// Process in bitsliced form
BitslicedSerpent serpent;
serpent.set_key(key);
serpent.encrypt_block(bitsliced);

// Convert back to traditional format
std::vector<std::array<uint8_t, 16>> result;
convert_from_bitsliced<128>(bitsliced, result);
```

### Serpent S-box in Bitsliced Form
```cpp
void sbox0(uint32_t& x0, uint32_t& x1, uint32_t& x2, uint32_t& x3) {
    uint32_t t0, t1, t2, t3;
    
    t0 = x0 & x1;
    t1 = x0 | x1;
    t2 = x2 ^ x3;
    t3 = x1 & x3;
    
    x0 = t0 ^ x2;
    x1 = t1 ^ t2;
    x2 = t2 ^ t3;
    x3 = x0 ^ x1 ^ x3;
}
```

## Test Vectors

The implementation includes test vectors for validation:

### Serpent Test Vector
- **Key**: `00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f 10 11 12 13 14 15 16 17 18 19 1a 1b 1c 1d 1e 1f`
- **Plaintext**: `00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f`
- **Expected Ciphertext**: `12 09 85 5a 0c 0d 0e 0f 08 09 0a 0b 0c 0d 0e 0f`

### AES Test Vector
- **Key**: `00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f`
- **Plaintext**: `00 11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff`

### DES Test Vector
- **Key**: `01 23 45 67 89 ab cd ef`
- **Plaintext**: `01 23 45 67 89 ab cd ef`

## Performance Considerations

### Theoretical Performance
- **Traditional Implementation**: O(n × r × s) operations
- **Bitsliced Implementation**: O(r × s) operations
- **Speedup**: O(n) times faster for n blocks

### Practical Considerations
- Bitslicing is most effective when processing multiple blocks simultaneously
- For single-block operations, conversion overhead may outweigh benefits
- Memory usage increases due to bitsliced representation

## Side-Channel Resistance

Bitsliced implementations provide natural resistance against timing attacks:

1. **Constant-Time Operations**: All operations take the same time regardless of input
2. **No Conditional Branches**: No secret-dependent control flow
3. **Data-Independent Memory Access**: Memory access patterns don't reveal secrets

## Advanced Topics

### Vectorization
Modern processors support SIMD instructions that can further accelerate bitsliced implementations:

```cpp
#include <immintrin.h>

void bitsliced_xor_avx2(__m256i& a, __m256i& b, __m256i& result) {
    result = _mm256_xor_si256(a, b);
}
```

### Boolean Function Optimization
S-boxes can be optimized using techniques like:
- Common subexpression elimination
- Gate-level optimization
- Circuit minimization

## References

1. Biham, E. (1997). A Fast New DES Implementation in Software.
2. Kwan, M. (2000). The Design of the ICE Encryption Algorithm.
3. Anderson, R., Biham, E., & Knudsen, L. (1998). Serpent: A Proposal for the Advanced Encryption Standard.
4. Daemen, J., & Rijmen, V. (2002). The Design of Rijndael: AES - The Advanced Encryption Standard.

## Educational Use

These materials are designed for educational purposes and include:
- Complete mathematical derivations
- Step-by-step implementation examples
- Test vectors for validation
- Performance analysis
- Security considerations

For production use, please refer to the full CryptoGL library implementations.

## Contributing

Contributions to improve the educational materials are welcome:
- Additional algorithms
- More detailed mathematical explanations
- Performance optimizations
- Additional test vectors
- Code examples in other languages

## License

This educational material is part of the CryptoGL project and follows the same licensing terms. 