#include "Nihilistes.hpp"

#include "MathematicalTools.hpp"

#include "exceptions/BadChar.hpp"
#include "exceptions/EmptyKey.hpp"

using namespace CryptoGL;

Nihilistes::Nihilistes(const KeyType &key, const KeyType &second_key, const ClassicalType &alpha)
    : SquareCipher(key, alpha)
{
    checkKey(second_key);
    this->second_key = second_key;
}

ClassicalType Nihilistes::encode(const ClassicalType &clear_text)
{
    const uint32_t clear_len = clear_text.length();
    const uint32_t second_key_len = second_key.length();
    ClassicalType crypted(clear_len * 2);

    uint32_t i = 0;
    for (const auto c : clear_text)
    {
        const auto coords_c = grid.getCharCoordinates(c);
        const auto coords_key = grid.getCharCoordinates(second_key[i]);

        const uint16_t final_value = (
            ((coords_c.y + 1) * 10) 
            + coords_c.x + 1 
            +((coords_key.y + 1) * 10) 
            + coords_key.x + 1
        ) % 100;

        // Since the final value must have 2 digits, we add a leading 0 if it has only a digit.
        if (final_value < 10)
        {
            crypted.push_back('0');
        }

        crypted.append(uint16::toString(final_value));
        i = (i + 1) % second_key_len;
    }

    return crypted;
}

ClassicalType Nihilistes::decode(const ClassicalType &cipher_text)
{
    const uint32_t cipher_len = cipher_text.length();
    const uint32_t second_key_len = second_key.length();
    ClassicalType decrypted(cipher_len / 2);

    uint32_t j = 0;
    for (uint32_t i = 0; i < cipher_len; i += 2)
    {
        const auto coords_key = grid.getCharCoordinates(second_key[j]);
        const uint8_t key_value = ((coords_key.y + 1) * 10) + coords_key.x + 1;
        const auto value = Maths::mod(atoi(cipher_text.substr(i, 2).c_str()) - key_value, 100);
        const uint8_t last_digit = value % 10;
        decrypted.push_back(grid.at(((value - last_digit) / 10) - 1, last_digit - 1));
        j = (j + 1) % second_key_len;
    }

    return decrypted;
}