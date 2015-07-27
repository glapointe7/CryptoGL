#include "XTEA.hpp"

#include "exceptions/BadKeyLength.hpp"

using namespace CryptoGL;

void XTEA::generateSubkeys()
{
    const UInt32Vector tmp_key = BigEndian32::toIntegersVector(key);
    uint32_t sum = 0;
    subkeys.resize(64);
    constexpr uint32_t DELTA = 0x9E3779B9;
    for (uint8_t i = 0; i < 32; ++i)
    {
        subkeys[i] = sum + tmp_key[sum % 4];
        sum += DELTA;
        subkeys[32 + i] = sum + tmp_key[(sum >> 11) % 4];
    }
}

uint32_t XTEA::F(const uint32_t half_block, const uint8_t round) const
{
    return (((half_block << 4) ^ (half_block >> 5)) + half_block) ^ subkeys[round];
}

void XTEA::encodeFeistelRounds(uint32_t &L, uint32_t &R, const uint8_t) const
{
    for (uint8_t i = 0; i < rounds; ++i)
    {
        L += F(R, i);
        R += F(L, i + 32);
    }
}

void XTEA::decodeFeistelRounds(uint32_t &L, uint32_t &R, const uint8_t) const
{
    for (int8_t i = rounds - 1; i >= 0; --i)
    {
        R -= F(L, i + 32);
        L -= F(R, i);
    }
}

void XTEA::processEncodingCurrentBlock()
{
    uint32_t L = current_block >> 32;
    uint32_t R = current_block & 0xFFFFFFFF;
    encodeFeistelRounds(L, R, 0);

    current_block = (static_cast<uint64_t> (L) << 32) | R;
}

void XTEA::processDecodingCurrentBlock()
{
    uint32_t L = current_block >> 32;
    uint32_t R = current_block & 0xFFFFFFFF;
    decodeFeistelRounds(L, R, 0);

    current_block = (static_cast<uint64_t> (L) << 32) | R;
}