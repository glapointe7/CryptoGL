#include "Polybius.hpp" 

using namespace CryptoGL;

ClassicalType Polybius::encode(const ClassicalType &clear_text)
{
    ClassicalType crypted(clear_text.length() * 2);
    for (const auto c : clear_text)
    {
        const Coordinates coords = grid.getCharCoordinates(c);
        crypted.push_back(coords.y + '1');
        crypted.push_back(coords.x + '1');
    }

    return crypted;
}

ClassicalType Polybius::decode(const ClassicalType &cipher_text)
{
    const uint32_t cipher_len = cipher_text.length();
    ClassicalType decrypted(cipher_len / 2);

    for (uint32_t i = 0; i < cipher_len; i += 2)
    {
        const Coordinates coords(cipher_text[i] - '1', cipher_text[i + 1] - '1');
        decrypted.push_back(grid.at(coords.x, coords.y));
    }

    return decrypted;
}