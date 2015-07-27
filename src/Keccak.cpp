#include "Keccak.hpp"

#include "Endian.hpp"

using namespace CryptoGL;

constexpr std::array<uint64_t, 24> Keccak::ROUND_CONSTANTS;
constexpr std::array<std::array<uint8_t, 5>, 5> Keccak::RHO_OFFSETS_TABLE;

void Keccak::applyRound(const uint8_t round_index)
{
    std::array<uint64_t, 5> C;
    std::array<uint64_t, 5> D;

    // Theta step.
    for (uint8_t x = 0; x < 5; ++x)
    {
        C[x] = state[x][0] 
             ^ state[x][1] 
             ^ state[x][2] 
             ^ state[x][3] 
             ^ state[x][4];
    }

    for (uint8_t x = 0; x < 5; ++x)
    {
        D[x] = C[(x + 4) % 5] ^ uint64::rotateLeft(C[(x + 1) % 5], 1);
    }

    for (uint8_t x = 0; x < 5; ++x)
    {
        for (uint8_t y = 0; y < 5; ++y)
        {
            state[x][y] ^= D[x];
        }
    }

    // Pi and rho steps.
    std::array<std::array<uint64_t, 5>, 5> B;
    for (uint8_t x = 0; x < 5; ++x)
    {
        for (uint8_t y = 0; y < 5; ++y)
        {
            B[y][(2 * x + 3 * y) % 5] = uint64::rotateLeft(state[x][y], RHO_OFFSETS_TABLE[x][y]);
        }
    }

    // Chi step.   
    for (uint8_t y = 0; y < 5; ++y)
    {
        for (uint8_t x = 0; x < 5; ++x)
        {
            state[x][y] = B[x][y] ^ ((~B[(x + 1) % 5][y]) & B[(x + 2) % 5][y]);
        }
    }

    // Iota step.
    state[0][0] ^= ROUND_CONSTANTS[round_index];
}

void Keccak::F()
{
    for (uint8_t i = 0; i < rounds; ++i)
    {
        applyRound(i);
    }
}

void Keccak::applyAbsorbingPhase(const BytesVector &padded_message)
{
    // If bitrate = 1024, then we iterate on blocks of 128 bytes + 72 bytes of 0.
    const uint64_t pad_len = padded_message.size();
    const uint16_t capacity_bytes = capacity / 8;
    for (uint64_t i = 0; i < pad_len; i += block_size)
    {
        BytesVector block = padded_message.range(i, i + block_size);
        block.insert(block.end(), capacity_bytes, 0);

        const UInt64Vector Pi = LittleEndian64::toIntegersVector(block);
        for (uint8_t x = 0; x < 5; ++x)
        {
            for (uint8_t y = 0; y < 5; ++y)
            {
                state[x][y] ^= Pi[x + 5 * y];
            }
        }

        F();
    }
}

BytesVector Keccak::applySqueezingPhase()
{
    BytesVector output;
    output.reserve(block_size);

    for (int16_t remaining_size = output_size; remaining_size > 0; remaining_size -= bitrate)
    {
        for (uint8_t x = 0; x < 5; ++x)
        {
            for (uint8_t y = 0; y < 5; ++y)
            {
                output.extend(LittleEndian64::toBytesVector(state[y][x]));
            }
        }
        if (remaining_size > 0)
        {
            F();
        }
    }

    return output.range(0, output_size / 8);
}