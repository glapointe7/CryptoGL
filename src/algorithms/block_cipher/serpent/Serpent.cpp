#include "Serpent.hpp"
#include "../../../core/types/Integer.hpp"
#include "../../../core/Utils/Endian.hpp"
#include "../../../core/types/Vector.hpp"
#include "../../../core/exceptions/BadKeyLength.hpp"
#include <array>
#include <algorithm>
#include <iostream>
#include <iomanip>


using namespace CryptoGL;


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
    X4 = X[1];
    X[1] |= X[2];
    X[1] ^= X[3];
    X4 ^= X[2];
    X[2] ^= X[1];
    X[3] |= X4;
    X[3] &= X[0];
    X4 ^= X[2];
    X[3] ^= X[1];
    X[1] |= X4;
    X[1] ^= X[0];
    X[0] |= X4;
    X[0] ^= X[2];
    X[1] ^= X4;
    X[2] ^= X[1];
    X[1] &= X[0];
    X[1] ^= X4;
    X[2] = ~X[2];
    X[2] |= X[0];
    X4 ^= X[2];
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
    X[0] ^= X[2];
    X4 &= X[2];
    X4 ^= X[0];
    X[0] &= X[1];
    X[1] ^= X[3];
    X[3] |= X4;
    X[2] ^= X[3];
    X[0] ^= X[3];
    X[1] ^= X4;
    X[3] &= X[2];
    X[3] ^= X[1];
    X[1] ^= X[0];
    X[1] |= X[2];
    X[0] ^= X[3];
    X[1] ^= X4;
    X[0] ^= X[1];
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

/*void Serpent::generateSubkeys()
{    
    // Step 1: Load padded user key into w[-8]..w[-1]
    UInt32Vector W = padKey();
    W.reserve(140);
    std::cout << "PADDED KEY" << std::endl;
    for (uint8_t i = 0; i < 8; ++i)
    {
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
        subkeys.extend(prekeys);
        std::cout << "subkeys[" << std::to_string(i) << "] = ";
        for (uint8_t n = 0; n < 4; ++n)
        {
            printToConsole(prekeys[n]);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}*/

// TO DO : With all 0s 256-bit key and 0s 128-bit plaintext, this still doesn't give the ciphertext from NESSIE.
// Loading: Reverse + little-endian or simply little-endian from NESSIE?
/*void Serpent::processEncodingCurrentBlock()
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
        current_block = S[round % 8](current_block);//applySBoxes(round % 8, current_block);
        for (uint8_t i = 0; i < 4; ++i)
        {
            printToConsole(current_block[i]);
        }
        std::cout << std::endl;

        std::cout << "LT - Block = ";
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
}*/

UInt32Vector Serpent::padKey()
{
    UInt32Vector padded_key = LittleEndian32::toIntegersVector(key);
    const uint8_t key_length = padded_key.size();
    if (key_length < 8)
    {
        padded_key.push_back(0x00000001);
        for (uint8_t i = 0; i < 7 - key_length; ++i)
        {
            padded_key.push_back(0);
        }
    }

    return padded_key;
}

// The key is in the NESSIE format so Big Endian.
void Serpent::setKey(const BytesVector &key)
{
    if (key.size() != 16 && key.size() != 24 && key.size() != 32)
        throw BadKeyLength("Serpent key must be 128-bit, 192-bit or 256-bit length.", key.size());

    this->key = key;
}

void Serpent::generateSubkeys()
{    
    // Step 1: Load padded user key into w[-8]..w[-1]
    UInt32Vector padded_key = padKey();
    std::array<uint32_t, 140> W;
    for (uint8_t i = 0; i < 8; ++i)
    {
        W[i] = padded_key[i];
    }
    
    // Step 2: Expand key to 132 32-bit words (w[0]..w[131])
    constexpr uint32_t PHI = 0x9E3779B9;
    for (uint8_t i = 8; i < 140; ++i)
    {
        W[i] = uint32::rotateLeft(W[i-8] ^ W[i-5] ^ W[i-3] ^ W[i-1] ^ PHI ^ (i-8), 11);
    }

    // Step 3: Generate 33 128-bit subkeys
    subkeys.reserve(132);
    for (uint8_t i = 0; i < rounds+1; ++i)
    { 
        const uint32_t j = 4 * i;
        const UInt32Vector prekeys = S[(rounds + 3-i) % 8]({W[j+8], W[j+9], W[j+10], W[j+11]});
        subkeys.extend(prekeys);
    }
}

void Serpent::processEncodingCurrentBlock()
{   
    // current_block: 128-bit input block in little-endian format.
    // subkeys: 33*4 words in little-endian format
    for (uint8_t round = 0; round < rounds - 1; ++round)
    {
        applyKeyMixing(round);
        current_block = S[round % 8](current_block);
        linearTransform();
    }

    // Round 31
    applyKeyMixing(rounds - 1);
    current_block = S[7](current_block);
    applyKeyMixing(rounds);
}

void Serpent::processDecodingCurrentBlock()
{
    applyKeyMixing(rounds);
    current_block = SI[7](current_block);
    applyKeyMixing(rounds - 1);
        
    // 2. 32 rounds in reverse
    for (int8_t round = rounds - 2; round >= 0; --round)
    {
        inverseLinearTransform();
        current_block = SI[round % 8](current_block);
        applyKeyMixing(round);
    }
}

void Serpent::applyKeyMixing(const uint8_t round)
{
    for (uint8_t i = 0; i < 4; ++i)
    {
        current_block[i] ^= subkeys[4*round + i];
    }
}

void Serpent::linearTransform()
{
    // Implements the Serpent linear transformation (bitslice form)
    // current_block is a Vector<uint32_t> of size 4: X0, X1, X2, X3
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
    current_block[2] ^= current_block[3] ^ (current_block[1] << 7);
    current_block[0] ^= current_block[1] ^ current_block[3];
    current_block[3] = uint32::rotateRight(current_block[3], 7);
    current_block[1] = uint32::rotateRight(current_block[1], 1);
    current_block[3] ^= current_block[2] ^ (current_block[0] << 3);
    current_block[1] ^= current_block[0] ^ current_block[2];
    current_block[2] = uint32::rotateRight(current_block[2], 3);
    current_block[0] = uint32::rotateRight(current_block[0], 13);
}