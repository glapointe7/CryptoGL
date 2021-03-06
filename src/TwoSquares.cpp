#include "TwoSquares.hpp"

using namespace CryptoGL;

TwoSquares::TwoSquares(const KeyType &key1, const KeyType &key2, const ClassicalType &alpha)
    : SquareCipher(key1, alpha), grid2(key2, alpha)
{
    checkKey(key2);
    this->key2 = key2;
}

ClassicalType TwoSquares::encode(const ClassicalType &clear_text)
{
    const ClassicalType full_text = appendChars(clear_text, 2, 'X');
    const uint32_t clear_len = full_text.length();
    ClassicalType crypted(clear_len);

    for (uint32_t i = 0; i < clear_len; i += 2)
    {
        const Coordinates coords_grid1 = grid.getCharCoordinates(clear_text[i]);
        const Coordinates coords_grid2 = grid2.getCharCoordinates(clear_text[i + 1]);

        // If the chars are on the same line, then there are encrypted by swapping them.
        if (coords_grid1.y != coords_grid2.y)
        {
            crypted.push_back(grid2.at(coords_grid1.y, coords_grid2.x));
            crypted.push_back(grid.at(coords_grid2.y, coords_grid1.x));
        }
        else
        {
            crypted.push_back(clear_text[i + 1]);
            crypted.push_back(clear_text[i]);
        }
    }

    return crypted;
}

ClassicalType TwoSquares::decode(const ClassicalType &cipher_text)
{
    const uint32_t cipher_len = cipher_text.length();
    ClassicalType decrypted(cipher_len);

    for (uint32_t i = 0; i < cipher_len; i += 2)
    {
        const Coordinates coords_grid1 = grid.getCharCoordinates(cipher_text[i + 1]);
        const Coordinates coords_grid2 = grid2.getCharCoordinates(cipher_text[i]);

        // If the chars are on the same line, then there are encrypted by swapping them.
        if (coords_grid1.y != coords_grid2.y)
        {
            decrypted.push_back(grid.at(coords_grid2.y, coords_grid1.x));
            decrypted.push_back(grid2.at(coords_grid1.y, coords_grid2.x));
        }
        else
        {
            decrypted.push_back(cipher_text[i + 1]);
            decrypted.push_back(cipher_text[i]);
        }
    }

    return decrypted;
}