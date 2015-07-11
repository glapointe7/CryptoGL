#include "MessageDigest.hpp"

#include "Bits.hpp"

using namespace CryptoGL;

constexpr std::array<uint8_t, 48> MD4::left_rotation_table;
constexpr std::array<uint8_t, 48> MD4::word_indexes;
constexpr std::array<uint32_t, 3> MD4::k;
constexpr std::array<uint8_t, 64> MD5::left_rotation_table;
constexpr std::array<uint32_t, 64> MD5::sine_magic_numbers;

void MD4::compress(UInt32Vector &state)
{
    UInt32Vector hash(state);
    for (uint8_t j = 0; j < rounds; ++j)
    {
        const uint8_t index = j >> 4;
        uint32_t f;
        switch (index)
        {
            case 0: f = F(hash[1], hash[2], hash[3]);
                break;
                
            case 1: f = G(hash[1], hash[2], hash[3]);
                break;
                
            case 2: f = H(hash[1], hash[2], hash[3]);
                break;
        }
        
        const uint32_t special_value = Bits::rotateLeft(hash[0] + f + current_block[word_indexes[j]] + k[index], left_rotation_table[j], 32);
        swapHashElements(hash, special_value);
    }

    applyDaviesMayerFunction(hash, state);
}

void MD5::compress(UInt32Vector &state)
{
    UInt32Vector hash(state);
    for (uint8_t j = 0; j < rounds; ++j)
    {
        const uint8_t index = j >> 4;
        uint32_t f, k;
        switch (index)
        {
            case 0: 
                f = F(hash[1], hash[2], hash[3]);
                k = j;
                break;
                
            case 1: 
                f = G(hash[1], hash[2], hash[3]);
                k = ((5 * j) + 1) % 16;
                break;
                
            case 2: 
                f = H(hash[1], hash[2], hash[3]);
                k = ((3 * j) + 5) % 16;
                break;
                
            case 3: 
                f = I(hash[1], hash[2], hash[3]);
                k = (7 * j) % 16;
                break;
        }

        const uint32_t special_value = hash[1] + Bits::rotateLeft(hash[0] + f + current_block[k] + sine_magic_numbers[j], left_rotation_table[j], 32);
        swapHashElements(hash, special_value);
    }

    applyDaviesMayerFunction(hash, state);
}

void MessageDigest::swapHashElements(UInt32Vector &hash, const uint32_t special_value)
{
    const uint32_t tmp = hash[3];
    hash[3] = hash[2];
    hash[2] = hash[1];
    hash[1] = special_value;
    hash[0] = tmp;
}