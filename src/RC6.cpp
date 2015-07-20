#include "RC6.hpp"

#include <algorithm>

#include "exceptions/BadKeyLength.hpp"

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
    // Initialize the expanded key table.
    const uint8_t subkeys_len = (rounds + 2) * 2;
    subkeys.resize(subkeys_len);
    subkeys[0] = P32;
    for (uint8_t i = 1; i < subkeys_len; ++i)
    {
        subkeys[i] = subkeys[i - 1] + Q32;
    }

    // This step mixes the secret key into the expanded key 'subkeys'.
    constexpr uint8_t int_size = 4;
    const uint8_t key_len = key.size();
    const uint8_t tmp_key_len = (key_len + int_size - 1) / int_size;
    const uint8_t k = 3 * std::max(tmp_key_len, subkeys_len);
    
    UInt32Vector tmp_key = LittleEndian32::toIntegersVector(key);
    uint32_t L = 0, R = 0;
    for (uint8_t l = 0, i = 0, j = 0; l < k; ++l)
    {
        L = subkeys[i] = uint32::rotateLeft((subkeys[i] + L + R) & 0xFFFFFFFF, 3);
        R = tmp_key[j] = uint32::rotateLeft(tmp_key[j] + L + R, (L + R) % 32);
        i = (i + 1) % subkeys_len;
        j = (j + 1) % tmp_key_len;
    }
}

uint64_t RC6::F(const uint64_t half_block, const uint8_t) const
{
    return uint32::rotateLeft((half_block * ((half_block * 2) + 1)) & 0xFFFFFFFF, 5);
}

void RC6::encodeFeistelRounds(uint64_t &L, uint64_t &R, const uint8_t) const
{
    std::array<uint32_t, 4> encoded_parts = {{
        static_cast<uint32_t>(L >> 32),
        static_cast<uint32_t>(L & 0xFFFFFFFF),
        static_cast<uint32_t>(R >> 32),
        static_cast<uint32_t>(R & 0xFFFFFFFF)
    }};

    encoded_parts[1] += subkeys[0];
    encoded_parts[3] += subkeys[1];
    for (uint8_t i = 1; i <= rounds; ++i)
    {
        const uint32_t u = F(encoded_parts[3], 0);
        const uint32_t t = F(encoded_parts[1], 0);
        const uint8_t j = i * 2;
        encoded_parts[0] = uint32::rotateLeft(encoded_parts[0] ^ t, u % 32) + subkeys[j];
        encoded_parts[2] = uint32::rotateLeft(encoded_parts[2] ^ u, t % 32) + subkeys[j + 1];
        
        for(uint8_t j = 0; j < 3; ++j)
        {
            std::iter_swap(encoded_parts.begin() + j, encoded_parts.begin() + j + 1);
        }
    }
    encoded_parts[0] += subkeys[(rounds * 2) + 2];
    encoded_parts[2] += subkeys[(rounds * 2) + 3];

    L = (static_cast<uint64_t> (encoded_parts[0]) << 32) | encoded_parts[1];
    R = (static_cast<uint64_t> (encoded_parts[2]) << 32) | encoded_parts[3];
}

void RC6::decodeFeistelRounds(uint64_t &L, uint64_t &R, const uint8_t) const
{
    std::array<uint32_t, 4> encoded_parts = {{
        static_cast<uint32_t>(L >> 32),
        static_cast<uint32_t>(L & 0xFFFFFFFF),
        static_cast<uint32_t>(R >> 32),
        static_cast<uint32_t>(R & 0xFFFFFFFF)
    }};

    encoded_parts[2] -= subkeys[(rounds * 2) + 3];
    encoded_parts[0] -= subkeys[(rounds * 2) + 2];
    for (uint8_t i = rounds; i > 0; --i)
    {
        for(uint8_t j = 0; j < 3; ++j)
        {
            std::iter_swap(encoded_parts.rbegin() + j, encoded_parts.rbegin() + j + 1);
        }
        
        const uint32_t u = F(encoded_parts[3], 0);
        const uint32_t t = F(encoded_parts[1], 0);
        const uint8_t j = i * 2;
        encoded_parts[2] = uint32::rotateRight((encoded_parts[2] - subkeys[j + 1]) & 0xFFFFFFFF, t % 32) ^ u;
        encoded_parts[0] = uint32::rotateRight((encoded_parts[0] - subkeys[j]) & 0xFFFFFFFF, u % 32) ^ t;
    }
    encoded_parts[3] -= subkeys[1];
    encoded_parts[1] -= subkeys[0];

    L = (static_cast<uint64_t> (encoded_parts[0]) << 32) | encoded_parts[1];
    R = (static_cast<uint64_t> (encoded_parts[2]) << 32) | encoded_parts[3];
}

void RC6::processEncodingCurrentBlock()
{
    uint64_t L = (static_cast<uint64_t> (current_block[0]) << 32) | current_block[1];
    uint64_t R = (static_cast<uint64_t> (current_block[2]) << 32) | current_block[3];
    encodeFeistelRounds(L, R, 0);

    current_block = {
        static_cast<uint32_t>(L >> 32),
        static_cast<uint32_t>(L & 0xFFFFFFFF),
        static_cast<uint32_t>(R >> 32),
        static_cast<uint32_t>(R & 0xFFFFFFFF)
    };
}

void RC6::processDecodingCurrentBlock()
{
    uint64_t L = (static_cast<uint64_t> (current_block[0]) << 32) | current_block[1];
    uint64_t R = (static_cast<uint64_t> (current_block[2]) << 32) | current_block[3];
    decodeFeistelRounds(L, R, 0);

    current_block = {
        static_cast<uint32_t>(L >> 32),
        static_cast<uint32_t>(L & 0xFFFFFFFF),
        static_cast<uint32_t>(R >> 32),
        static_cast<uint32_t>(R & 0xFFFFFFFF)
    };
}