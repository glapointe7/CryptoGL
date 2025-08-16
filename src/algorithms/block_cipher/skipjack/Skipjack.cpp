#include "Skipjack.hpp"

using namespace CryptoGL;

constexpr std::array<uint8_t, 256> Skipjack::f_table;

void Skipjack::setKey(const BytesVector &key)
{
    const uint8_t key_len = key.size();
    if (key_len != 10)
    {
        throw BadKeyLength("Your key has to be 10 bytes length.", key_len);
    }

    this->key = key;
}

void Skipjack::generateSubkeys()
{
    subkeys = key;
}

uint8_t Skipjack::F(const uint8_t half_block, const uint8_t round) const
{
    return f_table[half_block ^ subkeys[round]];
}

void Skipjack::encodeFeistelRounds(uint8_t& L, uint8_t& R, const uint8_t round) const
{
    L ^= F(R, round % 10);
    R ^= F(L, (round + 1) % 10);
    L ^= F(R, (round + 2) % 10);
    R ^= F(L, (round + 3) % 10);
}

void Skipjack::decodeFeistelRounds(uint8_t& L, uint8_t& R, const uint8_t round) const
{
    L ^= F(R, (round + 3) % 10);
    R ^= F(L, (round + 2) % 10);
    L ^= F(R, (round + 1) % 10);
    R ^= F(L, round % 10);
}

void Skipjack::applyRuleAOnBlock(const uint8_t round)
{
    const uint16_t tmp = current_block[3];
    current_block[3] = current_block[2];
    current_block[2] = current_block[1];
    uint8_t L = current_block[0] >> 8;
    uint8_t R = current_block[0] & 0xFF;
    encodeFeistelRounds(L, R, round * 4);
    current_block[1] = (L << 8) | R;
    current_block[0] = tmp ^ current_block[1] ^ (round + 1);
}

void Skipjack::applyRuleBOnBlock(const uint8_t round)
{
    const uint16_t tmp = current_block[3];
    current_block[3] = current_block[2];
    current_block[2] = current_block[0] ^ current_block[1] ^ (round + 1);
    uint8_t L = current_block[0] >> 8;
    uint8_t R = current_block[0] & 0xFF;
    encodeFeistelRounds(L, R, round * 4);
    current_block[1] = (L << 8) | R;
    current_block[0] = tmp;
}

void Skipjack::applyInverseRuleAOnBlock(const uint8_t round)
{
    const uint16_t tmp = current_block[0] ^ current_block[1] ^ (round + 1);
    uint8_t R = current_block[1] >> 8;
    uint8_t L = current_block[1] & 0xFF;
    decodeFeistelRounds(L, R, round * 4);
    current_block[0] = (R << 8) | L;
    current_block[1] = current_block[2];
    current_block[2] = current_block[3];
    current_block[3] = tmp;
}

void Skipjack::applyInverseRuleBOnBlock(const uint8_t round)
{
    const uint16_t tmp = current_block[0];
    uint8_t R = current_block[1] >> 8;
    uint8_t L = current_block[1] & 0xFF;
    decodeFeistelRounds(L, R, round * 4);
    current_block[0] = (R << 8) | L;
    current_block[1] = current_block[0] ^ current_block[2] ^ (round + 1);
    current_block[2] = current_block[3];
    current_block[3] = tmp;
}

void Skipjack::processEncodingCurrentBlock()
{
    for (uint8_t i = 0; i < 8; ++i)
        applyRuleAOnBlock(i);
    for (uint8_t i = 8; i < 16; ++i)
        applyRuleBOnBlock(i);
    for (uint8_t i = 16; i < 24; ++i)
        applyRuleAOnBlock(i);
    for (uint8_t i = 24; i < 32; ++i)
        applyRuleBOnBlock(i);
}

void Skipjack::processDecodingCurrentBlock()
{
    for (uint8_t i = 31; i >= 24; --i)
        applyInverseRuleBOnBlock(i);
    for (uint8_t i = 23; i >= 16; --i)
        applyInverseRuleAOnBlock(i);
    for (uint8_t i = 15; i >= 8; --i)
        applyInverseRuleBOnBlock(i);
    for (int8_t i = 7; i >= 0; --i)
        applyInverseRuleAOnBlock(i);
}