#include "XTEA.hpp"

#include "exceptions/BadKeyLength.hpp"

using namespace CryptoGL;

void XTEA::setKey(const BytesVector &key)
{
    const uint8_t key_len = key.size();
    if (key_len != 16)
    {
        throw BadKeyLength("Your key has to be 16 bytes length.", key_len);
    }

    this->key = key;
}

void XTEA::generateSubkeys()
{
    const UInt32Vector tmp_key = BigEndian32::toIntegersVector(key);
    uint32_t sum = 0;
    subkeys.resize(64);
    for (uint8_t i = 0; i < 32; ++i)
    {
        subkeys[i] = sum + tmp_key[sum % 4];
        sum += delta;
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

uint64_t XTEA::encodeBlock(const uint64_t &input)
{
    uint32_t L = input >> 32;
    uint32_t R = input & 0xFFFFFFFF;
    encodeFeistelRounds(L, R, 0);

    return (static_cast<uint64_t> (L) << 32) | R;
}

uint64_t XTEA::decodeBlock(const uint64_t &input)
{
    uint32_t L = input >> 32;
    uint32_t R = input & 0xFFFFFFFF;
    decodeFeistelRounds(L, R, 0);

    return (static_cast<uint64_t> (L) << 32) | R;
}