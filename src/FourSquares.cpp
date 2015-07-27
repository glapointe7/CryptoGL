#include "FourSquares.hpp"

using namespace CryptoGL;

FourSquares::FourSquares(const KeyType &key1, const KeyType &key2, const ClassicalType &alpha)
    : SquareCipher(key1, alpha), grid2(key2, alpha)
{
    checkKey(key2);
    this->key2 = key2;
}

ClassicalType FourSquares::encode(const ClassicalType &clear_text)
{
    const ClassicalType full_text = appendChars(clear_text, 2, 'X');
    const uint32_t clear_len = full_text.length();
    ClassicalType crypted;
    crypted.reserve(clear_len);
    
    const CipherGrid grid3(alpha);
    const CipherGrid grid4(grid3);

    for (uint32_t i = 0; i < clear_len; i += 2)
    {
        const auto coords_grid1 = grid3.getCharCoordinates(clear_text[i]);
        const auto coords_grid2 = grid4.getCharCoordinates(clear_text[i + 1]);
        crypted.push_back(grid.at(coords_grid1.y, coords_grid2.x));
        crypted.push_back(grid2.at(coords_grid2.y, coords_grid1.x));
    }

    return crypted;
}

ClassicalType FourSquares::decode(const ClassicalType &cipher_text)
{
    const uint32_t cipher_len = cipher_text.length();
    ClassicalType decrypted;
    decrypted.reserve(cipher_len);

    const CipherGrid grid3(alpha);
    const CipherGrid grid4(grid3);

    for (uint32_t i = 0; i < cipher_len; i += 2)
    {
        const auto coords_grid3 = grid.getCharCoordinates(cipher_text[i]);
        const auto coords_grid4 = grid2.getCharCoordinates(cipher_text[i + 1]);
        decrypted.push_back(grid3.at(coords_grid3.y, coords_grid4.x));
        decrypted.push_back(grid4.at(coords_grid4.y, coords_grid3.x));
    }

    return decrypted;
}