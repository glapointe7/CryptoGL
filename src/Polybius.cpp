#include "Polybius.hpp" 

using namespace CryptoGL;

ClassicalType Polybius::encode(const ClassicalType &clear_text)
{
    ClassicalType crypted(clear_text.length() * 2);
    for (const auto c : clear_text)
    {
        const Coordinates pos = grid.getCharCoordinates(c);
        crypted.push_back(pos.second + '1');
        crypted.push_back(pos.first + '1');
    }

    return crypted;
}

ClassicalType Polybius::decode(const ClassicalType &cipher_text)
{
    const uint32_t cipher_len = cipher_text.length();
    ClassicalType decrypted(cipher_len / 2);

    for (uint32_t i = 0; i < cipher_len; i += 2)
    {
        const Coordinates pos = std::make_pair(cipher_text[i] - '1', cipher_text[i + 1] - '1');
        decrypted.push_back(grid.at(pos.first, pos.second));
    }

    return decrypted;
}