#include "SHA1.hpp"

using namespace CryptoGL;

constexpr std::array<uint32_t, 4> SHA1::k;

void SHA1::compress(UInt32Vector &state)
{
    current_block.resize(rounds);

    // Extention of the 16 32-bit blocks to 80 blocks of 32 bits.
    for (uint8_t j = 16; j < rounds; ++j)
    {
        current_block[j] = uint32::rotateLeft(current_block[j - 3] 
                ^ current_block[j - 8] 
                ^ current_block[j - 14] 
                ^ current_block[j - 16], 1);
    }

    UInt32Vector hash(state);
    uint32_t f;
    for (uint8_t j = 0; j < rounds; ++j)
    {
        const uint8_t index = j / 20;
        switch (index)
        {
            case 0: f = ch(hash[1], hash[2], hash[3]);
                break;
            case 1:
            case 3: f = H(hash[1], hash[2], hash[3]);
                break;
            case 2: f = G(hash[1], hash[2], hash[3]);
                break;
        }

        // Special swapping of the hash.
        const uint32_t tmp = uint32::rotateLeft(hash[0], 5) + f + hash[4] + k[index] + current_block[j];
        hash[4] = hash[3];
        hash[3] = hash[2];
        hash[2] = uint32::rotateLeft(hash[1], 30);
        hash[1] = hash[0];
        hash[0] = tmp;
    }

    applyDaviesMayerFunction(hash, state);
}