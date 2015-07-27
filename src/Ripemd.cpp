#include "Ripemd.hpp"

#include <algorithm>

using namespace CryptoGL;

constexpr std::array<uint32_t, 5> Ripemd::MAGIC_NUMBERS_1;
constexpr std::array<uint32_t, 5> Ripemd::MAGIC_NUMBERS_3;
constexpr std::array<uint32_t, 4> Ripemd::MAGIC_NUMBERS_2;
constexpr std::array<uint8_t, 80> Ripemd::WORD_SELECTIONS_1;
constexpr std::array<uint8_t, 80> Ripemd::WORD_SELECTIONS_2;
constexpr std::array<uint8_t, 80> Ripemd::LEFT_SHIFTS_1;
constexpr std::array<uint8_t, 80> Ripemd::LEFT_SHIFTS_2;

void Ripemd::process128_256(UInt32Vector &hash, const uint8_t j) const
{
    const uint8_t index = j >> 4;
    uint32_t f1, f2;
    switch (index)
    {
        case 0:
            f1 = F(hash[1], hash[2], hash[3]);
            f2 = I(hash[5], hash[6], hash[7]);
            break;

        case 1:
            f1 = G(hash[1], hash[2], hash[3]);
            f2 = H(hash[5], hash[6], hash[7]);
            break;

        case 2:
            f1 = H(hash[1], hash[2], hash[3]);
            f2 = G(hash[5], hash[6], hash[7]);
            break;

        case 3:
            f1 = I(hash[1], hash[2], hash[3]);
            f2 = F(hash[5], hash[6], hash[7]);
            break;
    }

    uint32_t tmp = uint32::rotateLeft(hash[0] + f1 + current_block[WORD_SELECTIONS_1[j]] + MAGIC_NUMBERS_1[index], LEFT_SHIFTS_1[j], 32);
    hash[0] = hash[3];
    hash[3] = hash[2];
    hash[2] = hash[1];
    hash[1] = tmp;

    tmp = uint32::rotateLeft(hash[4] + f2 + current_block[WORD_SELECTIONS_2[j]] + MAGIC_NUMBERS_2[index], LEFT_SHIFTS_2[j], 32);
    hash[4] = hash[7];
    hash[7] = hash[6];
    hash[6] = hash[5];
    hash[5] = tmp;
}

void Ripemd::process160_320(UInt32Vector &hash, const uint8_t j) const
{
    const uint8_t index = j >> 4;
    uint32_t f1, f2;
    switch (index)
    {
        case 0:
            f1 = F(hash[1], hash[2], hash[3]);
            f2 = J(hash[6], hash[7], hash[8]);
            break;

        case 1:
            f1 = G(hash[1], hash[2], hash[3]);
            f2 = I(hash[6], hash[7], hash[8]);
            break;

        case 2:
            f1 = H(hash[1], hash[2], hash[3]);
            f2 = H(hash[6], hash[7], hash[8]);
            break;

        case 3:
            f1 = I(hash[1], hash[2], hash[3]);
            f2 = G(hash[6], hash[7], hash[8]);
            break;

        case 4:
            f1 = J(hash[1], hash[2], hash[3]);
            f2 = F(hash[6], hash[7], hash[8]);
            break;
    }

    uint32_t tmp = uint32::rotateLeft(hash[0] + f1 + current_block[WORD_SELECTIONS_1[j]] + MAGIC_NUMBERS_1[index], LEFT_SHIFTS_1[j], 32) + hash[4];
    hash[0] = hash[4];
    hash[4] = hash[3];
    hash[3] = uint32::rotateLeft(hash[2], 10, 32);
    hash[2] = hash[1];
    hash[1] = tmp;

    tmp = uint32::rotateLeft(hash[5] + f2 + current_block[WORD_SELECTIONS_2[j]] + MAGIC_NUMBERS_3[index], LEFT_SHIFTS_2[j], 32) + hash[9];
    hash[5] = hash[9];
    hash[9] = hash[8];
    hash[8] = uint32::rotateLeft(hash[7], 10, 32);
    hash[7] = hash[6];
    hash[6] = tmp;
}

void Ripemd128::compress(UInt32Vector &state)
{
    UInt32Vector hash(state);
    hash.reserve(8);
    hash.extend(state);
    for (uint8_t j = 0; j < rounds; ++j)
    {
        process128_256(hash, j);
    }

    const uint32_t tmp = state[1] + hash[2] + hash[7];
    state[1] = state[2] + hash[3] + hash[4];
    state[2] = state[3] + hash[0] + hash[5];
    state[3] = state[0] + hash[1] + hash[6];
    state[0] = tmp;
}

void Ripemd160::compress(UInt32Vector &state)
{
    UInt32Vector hash(state);
    hash.reserve(10);
    hash.extend(state);
    for (uint8_t j = 0; j < rounds; ++j)
    {
        process160_320(hash, j);
    }

    const uint32_t tmp = state[1] + hash[2] + hash[8];
    state[1] = state[2] + hash[3] + hash[9];
    state[2] = state[3] + hash[4] + hash[5];
    state[3] = state[4] + hash[0] + hash[6];
    state[4] = state[0] + hash[1] + hash[7];
    state[0] = tmp;
}

void Ripemd256::compress(UInt32Vector &state)
{
    UInt32Vector hash(state);
    for (uint8_t j = 0; j < rounds; ++j)
    {
        process128_256(hash, j);

        switch (j)
        {
            case 15: std::swap(hash[0], hash[4]);
                break;
            case 31: std::swap(hash[1], hash[5]);
                break;
            case 47: std::swap(hash[2], hash[6]);
                break;
            case 63: std::swap(hash[3], hash[7]);
                break;
        }
    }

    applyDaviesMayerFunction(hash, state);
}

void Ripemd320::compress(UInt32Vector &state)
{
    UInt32Vector hash(state);
    for (uint8_t j = 0; j < rounds; ++j)
    {
        process160_320(hash, j);

        switch (j)
        {
            case 15: std::swap(hash[1], hash[6]);
                break;
            case 31: std::swap(hash[3], hash[8]);
                break;
            case 47: std::swap(hash[0], hash[5]);
                break;
            case 63: std::swap(hash[2], hash[7]);
                break;
            case 79: std::swap(hash[4], hash[9]);
                break;
        }
    }

    applyDaviesMayerFunction(hash, state);
}