#include "Blowfish.hpp"

#include <algorithm>

#include "Endian.hpp"

#include "exceptions/BadKeyLength.hpp"

using namespace CryptoGL;

constexpr std::array<uint32_t, 18> Blowfish::P;

void Blowfish::setKey(const BytesVector &key)
{
    const uint8_t key_size = key.size();
    if (key_size < 6 || key_size > 56)
    {
        throw BadKeyLength("Your key length has to be between 6 bytes and 56 bytes.", key_size);
    }

    this->key = key;
}

void Blowfish::generateSubkeys()
{
    subkeys.reserve(18);
    const uint8_t key_len = key.size();

    for (uint8_t i = 0, j = 0; i < 18; ++i)
    {
        uint32_t data = 0;
        for (uint8_t k = 0; k < 4; ++k)
        {
            data = (data << 8) | key[j];
            j = (j + 1) % key_len;
        }
        subkeys.push_back(P[i] ^ data);
    }

    uint32_t L = 0;
    uint32_t R = 0;
    for (uint8_t i = 0; i < 18; i += 2)
    {
        encodeFeistelRounds(L, R, 0);
        subkeys[i] = L;
        subkeys[i + 1] = R;
    }

    for (uint8_t i = 0; i < 4; ++i)
    {
        for (uint16_t j = 0; j < 256; j += 2)
        {
            encodeFeistelRounds(L, R, 0);
            sbox[i][j] = L;
            sbox[i][j + 1] = R;
        }
    }
}

uint32_t Blowfish::F(const uint32_t half_block, const uint8_t) const
{
    return ((sbox[0][(half_block >> 24) & 0xFF] + sbox[1][(half_block >> 16) & 0xFF])
            ^ (sbox[2][(half_block >> 8) & 0xFF])) +sbox[3][half_block & 0xFF];
}

void Blowfish::encodeFeistelRounds(uint32_t &L, uint32_t &R, const uint8_t) const
{
    for (uint8_t i = 0; i != rounds; i += 2)
    {
        L ^= subkeys[i];
        R ^= F(L, subkeys[i]);
        R ^= subkeys[i + 1];
        L ^= F(R, subkeys[i]);
    }

    L ^= subkeys[rounds];
    R ^= subkeys[rounds + 1];
    std::swap(L, R);
}

void Blowfish::decodeFeistelRounds(uint32_t &L, uint32_t &R, const uint8_t) const
{
    for (uint8_t i = rounds + 1; i != 1; i -= 2)
    {
        L ^= subkeys[i];
        R ^= F(L, i);
        R ^= subkeys[i - 1];
        L ^= F(R, i);
    }

    L ^= subkeys[1];
    R ^= subkeys[0];
    std::swap(L, R);
}

uint64_t Blowfish::encodeBlock(const uint64_t &input)
{
    uint32_t L = input >> 32;
    uint32_t R = input & 0xFFFFFFFF;
    encodeFeistelRounds(L, R, 0);

    return (static_cast<uint64_t> (L) << 32) | R;
}

uint64_t Blowfish::decodeBlock(const uint64_t &input)
{
    uint32_t L = input >> 32;
    uint32_t R = input & 0xFFFFFFFF;
    decodeFeistelRounds(L, R, 0);

    return (static_cast<uint64_t> (L) << 32) | R;
}