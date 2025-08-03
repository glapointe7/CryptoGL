#include "Serpent.hpp"
#include "Integer.hpp"
#include "Endian.hpp"
#include "Vector.hpp"
#include "exceptions/BadKeyLength.hpp"
#include <array>
#include <algorithm>
#include <iostream>
#include <iomanip>


using namespace CryptoGL;

// S-Boxes (from Serpent specification)
/*const std::array<std::array<uint8_t, 16>, 8> Serpent::SBoxes = {{
    // S0
    {3, 8, 15, 1, 10, 6, 5, 11, 14, 13, 4, 2, 7, 0, 9, 12},
    // S1
    {15, 12, 2, 7, 9, 0, 5, 10, 1, 11, 14, 8, 6, 13, 3, 4},
    // S2
    {8, 6, 7, 9, 3, 12, 10, 15, 13, 1, 14, 4, 0, 11, 5, 2},
    // S3
    {0, 15, 11, 8, 12, 9, 6, 3, 13, 1, 2, 4, 10, 7, 5, 14},
    // S4 
    {1, 15, 8, 3, 12, 0, 11, 6, 2, 5, 4, 10, 9, 14, 7, 13},
    // S5 
    {15, 5, 2, 11, 4, 10, 9, 12, 0, 3, 14, 8, 13, 6, 7, 1},
    // S6  
    {7, 2, 12, 5, 8, 4, 6, 11, 14, 9, 1, 15, 13, 3, 10, 0},
    // S7 
    {1, 13, 15, 0, 14, 8, 2, 11, 7, 4, 12, 10, 9, 3, 5, 6}
}};

// Inverse S-Boxes (from Serpent specification)
const std::array<std::array<uint8_t, 16>, 8> Serpent::SBoxesInv = {{
    // S0^-1 
    {13, 3, 11, 0, 10, 6, 5, 12, 1, 14, 4, 7, 15, 9, 8, 2},
    // S1^-1 
    {5, 8, 2, 14, 15, 6, 12, 3, 11, 4, 7, 9, 1, 13, 10, 0},
    // S2^-1
    {12, 9, 15, 4, 11, 14, 1, 2, 0, 3, 6, 13, 5, 8, 10, 7},
    // S3^-1 
    {0, 9, 10, 7, 11, 14, 6, 13, 3, 5, 12, 2, 4, 8, 15, 1},
    // S4^-1 
    {5, 0, 8, 3, 10, 9, 7, 14, 2, 12, 11, 6, 4, 15, 13, 1},
    // S5^-1
    {8, 15, 2, 9, 4, 1, 13, 14, 11, 6, 5, 3, 7, 12, 10, 0},
    // S6^-1
    {15, 10, 1, 13, 5, 3, 6, 0, 4, 9, 14, 7, 2, 12, 8, 11},
    // S7^-1
    {3, 0, 6, 13, 9, 14, 15, 8, 5, 12, 11, 7, 10, 1, 4, 2}
}};*/

Serpent::Serpent(const BytesVector &key, const OperationModes mode, const BytesVector &IV)
    : BlockCipher(mode, 32, IV)
{
    setKey(key);
}

Serpent::Serpent(const BytesVector &key)
    : Serpent(key, OperationModes::ECB, {}) {}

Serpent::Serpent(const BytesVector &key, const OperationModes mode)
    : Serpent(key, mode, {}) {}


// https://www.ii.uib.no/~osvik/pub/aes3.pdf
// This is the bitsliced version
UInt32Vector Serpent::S0(const UInt32Vector &Y) 
{
    UInt32Vector X = Y;
    uint32_t X4;
    X[3] ^= X[0];
    X4 = X[1]; 
    X[1] &= X[3];
    X4 ^= X[2];
    X[1] ^= X[0];
    X[0] |= X[3]; 
    X[0] ^= X4;
    X4 ^= X[3]; 
    X[3] ^= X[2];
    X[2] |= X[1]; 
    X[2] ^= X4;
    X4 = ~X4; 
    X4 |= X[1];
    X[1] ^= X[3]; 
    X[1] ^= X4;
    X[3] |= X[0]; 
    X[1] ^= X[3];
    X4 ^= X[3];

    return {X[1], X4, X[2], X[0]};
}

UInt32Vector Serpent::S1(const UInt32Vector &Y) 
{
    UInt32Vector X = Y;
    uint32_t X4;
    X[0] = ~X[0];
    X[2] = ~X[2];
    X4 = X[0];
    X[0] &= X[1];
    X[2] ^= X[0];
    X[0] |= X[3];
    X[3] ^= X[2];
    X[1] ^= X[0];
    X[0] ^= X4;
    X4 |= X[1];
    X[1] ^= X[3];
    X[2] |= X[0];
    X[2] &= X4;
    X[0] ^= X[1];
    X[1] &= X[2];
    X[1] ^= X[0];
    X[0] &= X[2];
    X[0] ^= X4;
    return {X[2], X[0], X[3], X[1]};
}

UInt32Vector Serpent::S2(const UInt32Vector &Y) 
{
    UInt32Vector X = Y;
    uint32_t X4;
    X4 = X[0];   
    X[0] &= X[2];   
    X[0] ^= X[3];   
    X[2] ^= X[1];   
    X[2] ^= X[0];   
    X[3] |= X4;   
    X[3] ^= X[1];   
    X4 ^= X[2];   
    X[1] = X[3];   
    X[3] |= X4;   
    X[3] ^= X[0];   
    X[0] &= X[1];   
    X4 ^= X[0];   
    X[1] ^= X[3];   
    X[1] ^= X4;   
    X4 = ~X4;
    return {X[2], X[3], X[1], X4};
}

UInt32Vector Serpent::S3(const UInt32Vector &Y) 
{
    UInt32Vector X = Y;
    uint32_t X4;
    X4 = X[0];   
    X[0] |= X[3];   
    X[3] ^= X[1];   
    X[1] &= X4;   
    X4 ^= X[2];   
    X[2] ^= X[3];   
    X[3] &= X[0];   
    X4 |= X[1];   
    X[3] ^= X4;   
    X[0] ^= X[1];   
    X4 &= X[0];   
    X[1] ^= X[3];   
    X4 ^= X[2];   
    X[1] |= X[0];   
    X[1] ^= X[2];   
    X[0] ^= X[3];   
    X[2] = X[1];   
    X[1] |= X[3];   
    X[1] ^= X[0];
    return {X[1], X[2], X[3], X4};
}

UInt32Vector Serpent::S4(const UInt32Vector &Y) 
{
    UInt32Vector X = Y;
    uint32_t X4;
    X[1] ^= X[3];   
    X[3] = ~X[3];      
    X[2] ^= X[3];   
    X[3] ^= X[0];   
    X4 = X[1];   
    X[1] &= X[3];   
    X[1] ^= X[2];   
    X4 ^= X[3];   
    X[0] ^= X4;   
    X[2] &= X4;   
    X[2] ^= X[0];   
    X[0] &= X[1];   
    X[3] ^= X[0];   
    X4 |= X[1];   
    X4 ^= X[0];   
    X[0] |= X[3];   
    X[0] ^= X[2];   
    X[2] &= X[3];   
    X[0] = ~X[0];      
    X4 ^= X[2];
    return {X[1], X4, X[0], X[3]};
}

UInt32Vector Serpent::S5(const UInt32Vector &Y) 
{
    UInt32Vector X = Y;
    uint32_t X4;
    X[0] ^= X[1];   
    X[1] ^= X[3];   
    X[3] = ~X[3];      
    X4 = X[1];   
    X[1] &= X[0];   
    X[2] ^= X[3];   
    X[1] ^= X[2];   
    X[2] |= X4;   
    X4 ^= X[3];   
    X[3] &= X[1];   
    X[3] ^= X[0];   
    X4 ^= X[1];   
    X4 ^= X[2];   
    X[2] ^= X[0];   
    X[0] &= X[3];   
    X[2] = ~X[2];      
    X[0] ^= X4;   
    X4 |= X[3];   
    X[2] ^= X4;
    return {X[1], X[3], X[0], X[2]};
}

UInt32Vector Serpent::S6(const UInt32Vector &Y) 
{
    UInt32Vector X = Y;
    uint32_t X4;
    X[2] = ~X[2];      
    X4 = X[3];   
    X[3] &= X[0];   
    X[0] ^= X4;   
    X[3] ^= X[2];   
    X[2] |= X4;   
    X[1] ^= X[3];   
    X[2] ^= X[0];   
    X[0] |= X[1];   
    X[2] ^= X[1];   
    X4 ^= X[0];   
    X[0] |= X[3];   
    X[0] ^= X[2];   
    X4 ^= X[3];   
    X4 ^= X[0];   
    X[3] = ~X[3];      
    X[2] &= X4;   
    X[2] ^= X[3];
    return {X[0], X[1], X4, X[2]};
}

UInt32Vector Serpent::S7(const UInt32Vector &Y) 
{
    UInt32Vector X = Y;
    uint32_t X4;
    X4 = X[2];   
    X[2] &= X[1];   
    X[2] ^= X[3];   
    X[3] &= X[1];   
    X4 ^= X[2];   
    X[2] ^= X[1];   
    X[1] ^= X[0];   
    X[0] |= X4;   
    X[0] ^= X[2];   
    X[3] ^= X[1];   
    X[2] ^= X[3];   
    X[3] &= X[0];   
    X[3] ^= X4;   
    X4 ^= X[2];   
    X[2] &= X[0];   
    X4 = ~X4;      
    X[2] ^= X4;   
    X4 &= X[0];   
    X[1] ^= X[3];   
    X4 ^= X[1];
    return {X4, X[3], X[1], X[0]};
}

UInt32Vector Serpent::SI0(const UInt32Vector &Y) 
{ 
    UInt32Vector X = Y;
    uint32_t X4;
    X[2] = ~X[2];
    X4 = X[1];
    X[1] |= X[0];
    X4 = ~X4;
    X[1] ^= X[2];
    X[2] |= X4;
    X[1] ^= X[3];
    X[0] ^= X4;
    X[2] ^= X[0];
    X[0] &= X[3];
    X4 ^= X[0];
    X[0] |= X[1];
    X[0] ^= X[2];
    X[3] ^= X4;
    X[2] ^= X[1];
    X[3] ^= X[0];
    X[3] ^= X[1];
    X[2] &= X[3];
    X4 ^= X[2];

    return {X[0], X4, X[1], X[3]}; 
}

UInt32Vector Serpent::SI1(const UInt32Vector &Y) 
{ 
    UInt32Vector X = Y;
    uint32_t X4;
    X4 = X[1];   
    X[1] ^= X[3];   
    X[3] &= X[1];   
    X4 ^= X[2];   
    X[3] ^= X[0];   
    X[0] |= X[1];   
    X[2] ^= X[3];   
    X[0] ^= X4;   
    X[0] |= X[2];   
    X[1] ^= X[3];   
    X[0] ^= X[1];   
    X[1] |= X[3];   
    X[1] ^= X[0];   
    X4 = ~X4;      
    X4 ^= X[1];   
    X[1] |= X[0];   
    X[1] ^= X[0];   
    X[1] |= X4;   
    X[3] ^= X[1];
    return {X4, X[0], X[3], X[2]};
}

UInt32Vector Serpent::SI2(const UInt32Vector &Y) 
{ 
    UInt32Vector X = Y;
    uint32_t X4;
    X[2] ^= X[3];   
    X[3] ^= X[0];   
    X4 = X[3];   
    X[3] &= X[2];   
    X[3] ^= X[1];   
    X[1] |= X[2];   
    X[1] ^= X4;   
    X4 &= X[3];   
    X[2] ^= X[3];   
    X4 &= X[0];   
    X4 ^= X[2];   
    X[2] &= X[1];   
    X[2] |= X[0];   
    X[3] = ~X[3];   
    X[2] ^= X[3];   
    X[0] ^= X[3];   
    X[0] &= X[1];   
    X[3] ^= X4;   
    X[3] ^= X[0];
    return {X[1], X4, X[2], X[3]};
}

UInt32Vector Serpent::SI3(const UInt32Vector &Y) 
{ 
    UInt32Vector X = Y;
    uint32_t X4;
    X4 = X[2];   
    X[2] ^= X[1];   
    X[1] &= X[2];   
    X[1] ^= X[0];   
    X[0] &= X4;   
    X4 ^= X[3];   
    X[3] |= X[1];   
    X[3] ^= X[2];   
    X[0] ^= X4;   
    X[2] ^= X[0];   
    X[0] |= X[3];   
    X[0] ^= X[1];   
    X4 ^= X[2];   
    X[2] &= X[3];   
    X[1] |= X[3];   
    X[1] ^= X[2];   
    X4 ^= X[0];   
    X[2] ^= X4;
    return {X[2], X[1], X[3], X[0]};
}

UInt32Vector Serpent::SI4(const UInt32Vector &Y) 
{ 
    UInt32Vector X = Y;
    uint32_t X4;
    X4 = X[2];   
    X[2] &= X[3];   
    X[2] ^= X[1];   
    X[1] |= X[3];   
    X[1] &= X[0];   
    X4 ^= X[2];   
    X4 ^= X[1];   
    X[1] &= X[2];   
    X[0] = ~X[0];      
    X[3] ^= X4;   
    X[1] ^= X[3];   
    X[3] &= X[0];   
    X[3] ^= X[2];   
    X[0] ^= X[1];   
    X[2] &= X[0];   
    X[3] ^= X[0];   
    X[2] ^= X4;   
    X[2] |= X[3];   
    X[3] ^= X[0];   
    X[2] ^= X[1];
    return {X[0], X[3], X[2], X4};
}

UInt32Vector Serpent::SI5(const UInt32Vector &Y) 
{ 
    UInt32Vector X = Y;
    uint32_t X4;
    X[1] = ~X[1];      
    X4 = X[3];   
    X[2] ^= X[1];   
    X[3] |= X[0];   
    X[3] ^= X[2];   
    X[2] |= X[1];   
    X[2] &= X[0];   
    X4 ^= X[3];   
    X[2] ^= X4;   
    X4 |= X[0];   
    X4 ^= X[1];   
    X[1] &= X[2];   
    X[1] ^= X[3];   
    X4 ^= X[2];   
    X[3] &= X4;   
    X4 ^= X[1];   
    X[3] ^= X[0];   
    X[3] ^= X4;   
    X4 = ~X4;
    return {X[1], X4, X[3], X[2]};
}

UInt32Vector Serpent::SI6(const UInt32Vector &Y) 
{ 
    UInt32Vector X = Y;
    uint32_t X4;
    X[0] ^= X[2];   
    X4 = X[2];   
    X[2] &= X[0];   
    X4 ^= X[3];   
    X[2] = ~X[2];      
    X[3] ^= X[1];   
    X[2] ^= X[3];   
    X4 |= X[0];   
    X[0] ^= X[2];   
    X[3] ^= X4;   
    X4 ^= X[1];   
    X[1] &= X[3];   
    X[1] ^= X[0];   
    X[0] ^= X[3];   
    X[0] |= X[2];   
    X[3] ^= X[1];   
    X4 ^= X[0];
    return {X[1], X[2], X4, X[3]};
}

UInt32Vector Serpent::SI7(const UInt32Vector &Y) 
{ 
    UInt32Vector X = Y;
    uint32_t X4;
    X4 = X[2];   
    X[2] ^= X[0];   
    X[0] &= X[3];   
    X[2] = ~X[2];      
    X4 |= X[3];   
    X[3] ^= X[1];   
    X[1] |= X[0];   
    X[0] ^= X[2];   
    X[2] &= X4;   
    X[1] ^= X[2];   
    X[2] ^= X[0];   
    X[0] |= X[2];   
    X[3] &= X4;   
    X[0] ^= X[3];   
    X4 ^= X[1];   
    X[3] ^= X4;   
    X4 |= X[0];   
    X[3] ^= X[2];   
    X4 ^= X[2];

    return {X[3], X[0], X[1], X4};
}

void Serpent::printToConsole(const uint32_t x)
{
    std::cout << std::setw(8) << std::setfill('0') << std::hex << std::uppercase << x << " ";
}

// TODO : How this really works from NESSIE to padding the key? Reverse bytes then little-endian?
UInt32Vector Serpent::padKey()
{
    const uint8_t key_length = key.size();
    if (key_length < 32)
    {
        key.push_back(0x01);
        for (uint8_t i = 0; i < 31 - key_length; ++i)
        {
            key.push_back(0x00);
        }
    }

    return LittleEndian32::toIntegersVector(key);;
}

// The key is in the NESSIE format so Big Endian.
void Serpent::setKey(const BytesVector &key)
{
    if (key.size() != 16 && key.size() != 24 && key.size() != 32)
        throw BadKeyLength("Serpent key must be 128-bit, 192-bit or 256-bit length.", key.size());

    this->key = key;
    std::reverse(this->key.begin(), this->key.end());
}

void Serpent::generateSubkeys()
{    
    // Step 1: Load padded user key into w[-8]..w[-1] (little endian)
    UInt32Vector W = padKey();
    W.reserve(140);
    std::cout << "PADDED KEY" << std::endl;
    for (uint8_t i = 0; i < 8; ++i)
    {
        //W.push_back(LittleEndian32::toInteger(key.range(4*i, 4*i + 4)));
        printToConsole(W[i]);
    }
    std::cout << std::endl << std::endl;
    
    // Step 2: Expand key to 132 32-bit words (w[0]..w[131])
    constexpr uint32_t PHI = 0x9E3779B9;
    for (uint8_t i = 8; i < 140; ++i)
    {
        W.push_back(uint32::rotateLeft(W[i-8] ^ W[i-5] ^ W[i-3] ^ W[i-1] ^ PHI ^ (i-8), 11));
        std::cout << "W[" << std::to_string(i-8) << "] = ";
        printToConsole(W[i]);      
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // Step 3: Generate 33 128-bit subkeys
    subkeys.reserve(132);
    for (uint8_t i = 0; i < rounds+1; ++i)
    { 
        const uint32_t j = 4 * i;
        const UInt32Vector prekeys = S[(rounds + 3-i) % 8]({W[j+8], W[j+9], W[j+10], W[j+11]});
        std::cout << "subkeys[" << std::to_string(i) << "] = ";
        for (uint8_t n = 0; n < 4; ++n)
        {
            subkeys[j + n] = prekeys[n]; 
            printToConsole(prekeys[n]);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// TO DO : With all 0s 256-bit key and 0s 128-bit plaintext, this still doesn't give the ciphertext from NESSIE.
// Loading: Reverse + little-endian or simply little-endian from NESSIE?
void Serpent::processEncodingCurrentBlock()
{   
    // Implements Serpent block encryption (bitslice, 32 rounds)
    // current_block: 128-bit input block in big-endian format.
    // subkeys: 33*4 words in little-endian format
    std::cout << "PLAINTEXT LOADING" << std::endl;
    for (uint8_t i = 0; i < 4; ++i)
    {
        printToConsole(current_block[i]);
    }
    std::cout << std::endl << std::endl;

    for (uint8_t round = 0; round < rounds - 1; ++round)
    {
        std::cout << "ROUND " << std::to_string(round) << std::endl;
        std::cout << "Key Mixing - Block = ";
        applyKeyMixing(round);
        std::cout << std::endl;

        std::cout << "S[" << std::to_string(round % 8) << "] - Block = ";
        current_block = S[round % 8](current_block);
        for (uint8_t i = 0; i < 4; ++i)
        {
            printToConsole(current_block[i]);
        }
        std::cout << std::endl;

        std::cout << "LT - Block = ";
        //current_block = linearTransform(current_block);
        linearTransform();
        for (uint8_t i = 0; i < 4; ++i)
        {
            printToConsole(current_block[i]);
        }
        std::cout << std::endl << std::endl;
    }

    // Round 31
    std::cout << "ROUND 31" << std::endl;
    std::cout << "Key Mixing - Block = ";
    applyKeyMixing(rounds - 1);
    std::cout << std::endl;

    std::cout << "S[7] - Block = ";
    current_block = S[7](current_block);
    for (uint8_t i = 0; i < 4; ++i)
    {
        printToConsole(current_block[i]);
    }
    std::cout << std::endl;

    std::cout << "Key Mixing - Block = ";
    applyKeyMixing(rounds);
    std::cout << std::endl << std::endl;
 
    for (uint8_t i = 0; i < 4; ++i)
    {
        printToConsole(Integer<uint32_t>::bytesSwap(current_block[i]));
    }
    std::cout << std::endl << std::endl;
}

void Serpent::applyKeyMixing(const uint8_t round)
{
    for (uint8_t i = 0; i < 4; ++i)
    {
        current_block[i] ^= subkeys[4*round + i];
        printToConsole(current_block[i]);
    }
}

void Serpent::processDecodingCurrentBlock()
{
    applyKeyMixing(rounds);
        
    // 2. 32 rounds in reverse
    for (uint8_t round = rounds - 1; round >= 1; --round)
    {
        current_block = SI[round % 8](current_block);
        applyKeyMixing(round);
        inverseLinearTransform();
    }

    current_block = SI[0](current_block);
    applyKeyMixing(0);
}

//UInt32Vector Serpent::linearTransform(const UInt32Vector &block)
void Serpent::linearTransform()
{
    // Implements the Serpent linear transformation (bitslice form)
    // current_block is a Vector<uint32_t> of size 4: X0, X1, X2, X3
    /*uint32_t X0 = uint32::rotateLeft(block[0], 13);
    uint32_t X2 = uint32::rotateLeft(block[2], 3);
    uint32_t X1 = block[1] ^ X0 ^ X2;
    uint32_t X3 = block[3] ^ X2 ^ (X0 << 3);
    X1 = uint32::rotateLeft(X1, 1);
    X3 = uint32::rotateLeft(X3, 7);
    X0 = X0 ^ X1 ^ X3;
    X2 = X2 ^ X3 ^ (X1 << 7);
    X0 = uint32::rotateLeft(X0, 5);
    X2 = uint32::rotateLeft(X2, 22);

    return {X0, X1, X2, X3};*/
    current_block[0] = uint32::rotateLeft(current_block[0], 13);
    current_block[2] = uint32::rotateLeft(current_block[2], 3);
    current_block[1] ^= current_block[0] ^ current_block[2];
    current_block[3] = current_block[3] ^ current_block[2] ^ (current_block[0] << 3);
    current_block[1] = uint32::rotateLeft(current_block[1], 1);
    current_block[3] = uint32::rotateLeft(current_block[3], 7);
    current_block[0] ^= current_block[1] ^ current_block[3];
    current_block[2] = current_block[2] ^ current_block[3] ^ (current_block[1] << 7);
    current_block[0] = uint32::rotateLeft(current_block[0], 5);
    current_block[2] = uint32::rotateLeft(current_block[2], 22);
}

void Serpent::inverseLinearTransform()
{
    // Implements the Serpent inverse linear transformation (bitslice form)
    // current_block is a Vector<uint32_t> of size 4: X0, X1, X2, X3
    current_block[2] = uint32::rotateRight(current_block[2], 22);
    current_block[0] = uint32::rotateRight(current_block[0], 5);
    current_block[2] ^= current_block[3] ^ ((current_block[1] << 7) & 0xFFFFFFFF);
    current_block[0] ^= current_block[1] ^ current_block[3];
    current_block[3] = uint32::rotateRight(current_block[3], 7);
    current_block[1] = uint32::rotateRight(current_block[1], 1);
    current_block[3] ^= current_block[2] ^ ((current_block[0] << 3) & 0xFFFFFFFF);
    current_block[1] ^= current_block[0] ^ current_block[2];
    current_block[2] = uint32::rotateRight(current_block[2], 3);
    current_block[0] = uint32::rotateRight(current_block[0], 13);
}