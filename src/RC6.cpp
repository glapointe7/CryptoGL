#include "RC6.hpp"

#include <algorithm>

#include "exceptions/BadKeyLength.hpp"

#include "Bits.hpp"

using namespace CryptoGL;

void RC6::setKey(const BytesVector &key)
{
    const uint8_t key_len = key.size();
    if (key_len != 16 && key_len != 24 && key_len != 32)
    {
        throw BadKeyLength("Your key has to be 16, 24 or 32 bytes length.", key_len);
    }

    this->key = key;
}

void RC6::generateSubkeys()
{
    constexpr uint8_t int_size = 4;
    const uint8_t key_len = key.size();
    const uint8_t tmp_key_len = (key_len + int_size - 1) / int_size;

    UInt32Vector tmp_key = LittleEndian32::toIntegersVector(key);

    // Initialize the expanded key table.
    const uint8_t subkeys_len = (rounds + 2) * 2;
    subkeys.resize(subkeys_len);
    subkeys[0] = P32;
    for (uint8_t i = 1; i < subkeys_len; ++i)
    {
        subkeys[i] = subkeys[i - 1] + Q32;
    }

    // This step mixes the secret key into the expanded key 'subkeys'.
    const uint8_t k = 3 * std::max(tmp_key_len, subkeys_len);
    uint32_t L = 0, R = 0;
    for (uint8_t l = 0, i = 0, j = 0; l < k; ++l)
    {
        L = subkeys[i] = Bits::rotateLeft((subkeys[i] + L + R) & 0xFFFFFFFF, 3);
        R = tmp_key[j] = Bits::rotateLeft(tmp_key[j] + L + R, (L + R) % 32);
        i = (i + 1) % subkeys_len;
        j = (j + 1) % tmp_key_len;
    }
}

uint64_t RC6::F(const uint64_t half_block, const uint8_t) const
{
    return Bits::rotateLeft((half_block * ((half_block * 2) + 1)) & 0xFFFFFFFF, 5);
}

void RC6::encodeFeistelRounds(uint64_t &L, uint64_t &R, const uint8_t) const
{
    uint32_t A = L >> 32;
    uint32_t B = L & 0xFFFFFFFF;
    uint32_t C = R >> 32;
    uint32_t D = R & 0xFFFFFFFF;

    B += subkeys[0];
    D += subkeys[1];
    for (uint8_t i = 1; i <= rounds; ++i)
    {
        const uint8_t j = i * 2;
        uint32_t t = F(B, 0);
        const uint32_t u = F(D, 0);
        A = Bits::rotateLeft(A ^ t, u % 32) + subkeys[j];
        C = Bits::rotateLeft(C ^ u, t % 32) + subkeys[j + 1];
        t = A;
        A = B;
        B = C;
        C = D;
        D = t;
    }
    A += subkeys[(rounds * 2) + 2];
    C += subkeys[(rounds * 2) + 3];

    L = (static_cast<uint64_t> (A) << 32) | B;
    R = (static_cast<uint64_t> (C) << 32) | D;
}

void RC6::decodeFeistelRounds(uint64_t &L, uint64_t &R, const uint8_t) const
{
    uint32_t A = L >> 32;
    uint32_t B = L & 0xFFFFFFFF;
    uint32_t C = R >> 32;
    uint32_t D = R & 0xFFFFFFFF;

    C -= subkeys[(rounds * 2) + 3];
    A -= subkeys[(rounds * 2) + 2];
    for (uint8_t i = rounds; i > 0; --i)
    {
        const uint8_t j = i * 2;
        uint32_t t = A;
        A = D;
        D = C;
        C = B;
        B = t;
        const uint32_t u = F(D, 0);
        t = F(B, 0);
        C = Bits::rotateRight((C - subkeys[j + 1]) & 0xFFFFFFFF, t % 32) ^ u;
        A = Bits::rotateRight((A - subkeys[j]) & 0xFFFFFFFF, u % 32) ^ t;
    }
    D -= subkeys[1];
    B -= subkeys[0];

    L = (static_cast<uint64_t> (A) << 32) | B;
    R = (static_cast<uint64_t> (C) << 32) | D;
}

void RC6::processEncodingCurrentBlock()
{
    uint64_t L = (static_cast<uint64_t> (current_block[0]) << 32) | current_block[1];
    uint64_t R = (static_cast<uint64_t> (current_block[2]) << 32) | current_block[3];
    encodeFeistelRounds(L, R, 0);

    current_block[0] = L >> 32;
    current_block[1] = L & 0xFFFFFFFF;
    current_block[2] = R >> 32;
    current_block[3] = R & 0xFFFFFFFF;
}

void RC6::processDecodingCurrentBlock()
{
    uint64_t L = (static_cast<uint64_t> (current_block[0]) << 32) | current_block[1];
    uint64_t R = (static_cast<uint64_t> (current_block[2]) << 32) | current_block[3];
    decodeFeistelRounds(L, R, 0);

    current_block[0] = L >> 32;
    current_block[1] = L & 0xFFFFFFFF;
    current_block[2] = R >> 32;
    current_block[3] = R & 0xFFFFFFFF;
}