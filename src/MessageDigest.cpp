#include "MessageDigest.hpp"

using namespace CryptoGL;

constexpr std::array<uint8_t, 48> MD4::LEFT_ROTATIONS;
constexpr std::array<uint8_t, 48> MD4::WORD_INDICES;
constexpr std::array<uint32_t, 3> MD4::K;
constexpr std::array<uint8_t, 64> MD5::LEFT_ROTATIONS;
constexpr std::array<uint32_t, 64> MD5::SINE_MAGIC_NUMBERS;

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
        
        const uint32_t special_value = uint32::rotateLeft(hash[0] + f + current_block[WORD_INDICES[j]] + K[index], LEFT_ROTATIONS[j], 32);
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

        const uint32_t special_value = hash[1] + uint32::rotateLeft(hash[0] + f + current_block[k] + SINE_MAGIC_NUMBERS[j], LEFT_ROTATIONS[j], 32);
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