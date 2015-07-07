#include "RC2.hpp"

#include "Bits.hpp"

using namespace CryptoGL;

constexpr std::array<uint8_t, 256> RC2::pi_table;
constexpr std::array<uint8_t, 4> RC2::mixup_rotation;

void RC2::setKey(const BytesVector &key)
{
    const uint8_t key_len = key.size();
    if (key_len > 16 || key_len == 0)
    {
        throw BadKeyLength("Your key has to be between 1 and 16 bytes length.", key_len);
    }

    this->key = key;
}

void RC2::generateSubkeys()
{
    BytesVector tmp_subkeys(key);
    tmp_subkeys.reserve(128);
    const uint8_t key_len = key.size();

    for (uint8_t i = key_len; i < 128; ++i)
    {
        tmp_subkeys[i] = pi_table[(tmp_subkeys[i - key_len] + tmp_subkeys[i - 1]) & 0xFF];
    }

    const uint8_t T1 = key_len * 8;
    const uint8_t TM = 0xFF >> (7 & -T1);
    const uint8_t T8 = (T1 + 7) / 8;

    tmp_subkeys[128 - T8] = pi_table[tmp_subkeys[128 - T8] & TM];
    for (int8_t i = 127 - T8; i >= 0; --i)
    {
        tmp_subkeys[i] = pi_table[tmp_subkeys[i + 1] ^ tmp_subkeys[i + T8]];
    }

    // Get the 16-bits subkeys in little-endian.
    subkeys.reserve(64);
    for (uint8_t i = 0; i < 128; i += 2)
    {
        subkeys.push_back(LittleEndian16::toIntegerRange(tmp_subkeys, i, i + 2));
    }
}

void RC2::mixUp(const uint8_t index, const uint8_t key_index)
{
    const uint8_t i = 4 + index;
    current_block[index] += subkeys[key_index + index] + (current_block[(i - 2) % 4] & current_block[(i - 1) % 4]) + (current_block[(i - 3) % 4] & ~current_block[(i - 1) % 4]);
    current_block[index] = Bits::rotateLeft(current_block[index], mixup_rotation[index], 16);
}

void RC2::mash(const uint8_t index)
{
    current_block[index] += subkeys[current_block[(index + 3) % 4] % 64];
}

void RC2::inverseMixUp(const uint8_t index, const uint8_t key_index)
{
    const uint8_t i = 4 + index;
    current_block[index] = Bits::rotateRight(current_block[index], mixup_rotation[index], 16);
    current_block[index] -= subkeys[key_index + index] + (current_block[(i - 2) % 4] & current_block[(i - 1) % 4]) + (current_block[(i - 3) % 4] & ~current_block[(i - 1) % 4]);
}

void RC2::inverseMash(const uint8_t index)
{
    current_block[index] -= subkeys[current_block[(index + 3) % 4] % 64];
}

void RC2::processEncodingCurrentBlock()
{
    for (uint8_t i = 0; i < rounds; ++i)
    {
        const uint8_t k = i * 4;
        for (uint8_t j = 0; j < 4; ++j)
        {
            mixUp(j, k);
        }

        if (i == 4 || i == 10)
        {
            for (uint8_t j = 0; j < 4; ++j)
            {
                mash(j);
            }
        }
    }
}

void RC2::processDecodingCurrentBlock()
{
    for (int8_t i = rounds - 1; i >= 0; --i)
    {
        const uint8_t k = i * 4;
        if (i == 4 || i == 10)
        {
            for (int8_t j = 3; j >= 0; --j)
            {
                inverseMash(j);
            }
        }

        for (int8_t j = 3; j >= 0; --j)
        {
            inverseMixUp(j, k);
        }
    }
}