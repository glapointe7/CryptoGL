#include "AES.hpp"

#include "exceptions/BadKeyLength.hpp"

#include <algorithm>

using namespace CryptoGL;

constexpr std::array<uint8_t, 16> AES::shift_indexes;
constexpr std::array<uint8_t, 16> AES::inverse_shift_indexes;
constexpr std::array<uint32_t, 15> AES::round_constants;
constexpr std::array<uint8_t, 256> AES::sbox;
constexpr std::array<uint8_t, 256> AES::inverse_sbox;

constexpr std::array<uint8_t, 256> AES::table_2;
constexpr std::array<uint8_t, 256> AES::table_3;
constexpr std::array<uint8_t, 256> AES::table_9;
constexpr std::array<uint8_t, 256> AES::table_11;
constexpr std::array<uint8_t, 256> AES::table_13;
constexpr std::array<uint8_t, 256> AES::table_14;

void AES::setKey(const BytesVector &key)
{
    const uint8_t key_len = key.size();
    if (key_len != 16 && key_len != 24 && key_len != 32)
    {
        throw BadKeyLength("Your key has to be 128, 192 or 256 bits length.", key_len);
    }

    switch (key_len)
    {
        case 24: 
            rounds = 12;
            break;
            
        case 32: 
            rounds = 14;
            break;
    }
    this->key = key;
}

void AES::subBytes(const std::array<uint8_t, 256> &box)
{
    for (uint8_t i = 0; i < 4; ++i)
    {
        current_block[i] = 
                  box[(current_block[i] >> 24) & 0xFF] << 24
                | box[(current_block[i] >> 16) & 0xFF] << 16
                | box[(current_block[i] >> 8) & 0xFF] << 8
                | box[current_block[i] & 0xFF];
    }
}

void AES::shiftRows()
{
    current_block = {
          ((current_block[0] >> 24) & 0xFF) << 24
        | ((current_block[1] >> 16) & 0xFF) << 16
        | ((current_block[2] >> 8) & 0xFF) << 8
        |  (current_block[3] & 0xFF),
        
          ((current_block[1] >> 24) & 0xFF) << 24
        | ((current_block[2] >> 16) & 0xFF) << 16
        | ((current_block[3] >> 8) & 0xFF) << 8
        |  (current_block[0] & 0xFF),
        
          ((current_block[2] >> 24) & 0xFF) << 24
        | ((current_block[3] >> 16) & 0xFF) << 16
        | ((current_block[0] >> 8) & 0xFF) << 8
        |  (current_block[1] & 0xFF),
        
          ((current_block[3] >> 24) & 0xFF) << 24
        | ((current_block[0] >> 16) & 0xFF) << 16
        | ((current_block[1] >> 8) & 0xFF) << 8
        |  (current_block[2] & 0xFF)
    };
}

void AES::inverseShiftRows()
{
    current_block = {
          ((current_block[0] >> 24) & 0xFF) << 24
        | ((current_block[3] >> 16) & 0xFF) << 16
        | ((current_block[2] >> 8) & 0xFF) << 8
        |  (current_block[1] & 0xFF),
        
          ((current_block[1] >> 24) & 0xFF) << 24
        | ((current_block[0] >> 16) & 0xFF) << 16
        | ((current_block[3] >> 8) & 0xFF) << 8
        |  (current_block[2] & 0xFF),
        
          ((current_block[2] >> 24) & 0xFF) << 24
        | ((current_block[1] >> 16) & 0xFF) << 16
        | ((current_block[0] >> 8) & 0xFF) << 8
        |  (current_block[3] & 0xFF),
        
          ((current_block[3] >> 24) & 0xFF) << 24
        | ((current_block[2] >> 16) & 0xFF) << 16
        | ((current_block[1] >> 8) & 0xFF) << 8
        |  (current_block[0] & 0xFF)
    };
}

void AES::mixColumns()
{
    for (uint8_t i = 0; i < 4; ++i)
    {
        current_block[i] = ((table_2[(current_block[i] >> 24) & 0xFF]
                         ^ table_3[(current_block[i] >> 16) & 0xFF]
                         ^ ((current_block[i] >> 8) & 0xFF)
                         ^ (current_block[i] & 0xFF)) << 24)

                     |     ((((current_block[i] >> 24) & 0xFF)
                         ^ table_2[(current_block[i] >> 16) & 0xFF]
                         ^ table_3[(current_block[i] >> 8) & 0xFF]
                         ^ (current_block[i] & 0xFF)) << 16)

                     |     ((((current_block[i] >> 24) & 0xFF)
                         ^ ((current_block[i] >> 16) & 0xFF)
                         ^ table_2[(current_block[i] >> 8) & 0xFF]
                         ^ table_3[current_block[i] & 0xFF]) << 8)

                     |     (table_3[(current_block[i] >> 24) & 0xFF]
                         ^ ((current_block[i] >> 16) & 0xFF)
                         ^ ((current_block[i] >> 8) & 0xFF)
                         ^ table_2[current_block[i] & 0xFF]);
    }
}

void AES::inverseMixColumns()
{
    for (uint8_t i = 0; i < 4; ++i)
    {
        current_block[i] = ((table_14[(current_block[i] >> 24) & 0xFF]
                         ^ table_11[(current_block[i] >> 16) & 0xFF]
                         ^ table_13[(current_block[i] >> 8) & 0xFF]
                         ^ table_9[current_block[i] & 0xFF]) << 24)

                     |     ((table_9[(current_block[i] >> 24) & 0xFF]
                         ^ table_14[(current_block[i] >> 16) & 0xFF]
                         ^ table_11[(current_block[i] >> 8) & 0xFF]
                         ^ table_13[current_block[i] & 0xFF]) << 16)

                     |     ((table_13[(current_block[i] >> 24) & 0xFF]
                         ^ table_9[(current_block[i] >> 16) & 0xFF]
                         ^ table_14[(current_block[i] >> 8) & 0xFF]
                         ^ table_11[current_block[i] & 0xFF]) << 8)

                     |     (table_11[(current_block[i] >> 24) & 0xFF]
                         ^ table_13[(current_block[i] >> 16) & 0xFF]
                         ^ table_9[(current_block[i] >> 8) & 0xFF]
                         ^ table_14[current_block[i] & 0xFF]);
    }
}

constexpr uint32_t AES::subWord(const uint32_t word)
{
    return sbox[word & 0xFF]
        | (sbox[(word >> 8) & 0xFF]) << 8
        | (sbox[(word >> 16) & 0xFF]) << 16
        | (sbox[(word >> 24) & 0xFF]) << 24;
}

void AES::generateSubkeys()
{
    const uint8_t max_round = (rounds + 1) * 4;
    subkeys.reserve(max_round);
    subkeys = BigEndian32::toIntegersVector(key);

    const uint8_t Nk = key.size() / 4;
    for (uint8_t i = Nk; i < max_round; ++i)
    {
        uint32_t tmp = subkeys[i - 1];
        if (!(i % Nk))
        {
            tmp = subWord(uint32::rotateLeft(tmp, 8)) ^ round_constants[(i / Nk) - 1];
        }
        else if (Nk > 6 && i % Nk == 4)
        {
            tmp = subWord(tmp);
        }
        subkeys.push_back(subkeys[i - Nk] ^ tmp);
    }
}

void AES::addRoundKey(const uint8_t round)
{
    const uint8_t j = round * 4;
    for (uint8_t i = 0; i < 4; ++i)
    {
        current_block[i] ^= subkeys[j + i];
    }
}

void AES::processEncodingCurrentBlock()
{
    addRoundKey(0);

    for (uint8_t i = 1; i < rounds; ++i)
    {
        subBytes(sbox);
        shiftRows();
        mixColumns();
        addRoundKey(i);
    }

    // Last round : no mixColumns.
    subBytes(sbox);
    shiftRows();
    addRoundKey(rounds);
}

void AES::processDecodingCurrentBlock()
{
    addRoundKey(rounds);

    for (uint8_t i = rounds - 1; i >= 1; --i)
    {
        inverseShiftRows();
        subBytes(inverse_sbox);
        addRoundKey(i);
        inverseMixColumns();
    }

    inverseShiftRows();
    subBytes(inverse_sbox);
    addRoundKey(0);
}