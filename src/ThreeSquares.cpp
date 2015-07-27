#include "ThreeSquares.hpp"

using namespace CryptoGL;

ThreeSquares::ThreeSquares(const KeyType &key1, const KeyType &key2, const KeyType &key3, const ClassicalType &alpha)
    : SquareCipher(key1, alpha), grid2(key2, alpha), grid3(key3, alpha)
{
    checkKey(key2);
    checkKey(key3);
    this->key2 = key2;
    this->key3 = key3;
}

ClassicalType ThreeSquares::encode(const ClassicalType &clear_text)
{
    const ClassicalType full_text = appendChars(clear_text, 2, 'X');
    const uint32_t clear_len = full_text.length();
    ClassicalType crypted(3 * clear_len / 2);

    srand(time(0));

    for (uint32_t i = 0; i < clear_len; i += 2)
    {
        const auto coords_grid1 = grid.getCharCoordinates(clear_text[i]);
        const auto coords_grid2 = grid2.getCharCoordinates(clear_text[i + 1]);
        const auto random_index = rand() % grid.getDimension();

        crypted.push_back(grid.at(random_index, coords_grid1.x));
        crypted.push_back(grid3.at(coords_grid1.y, coords_grid2.x));
        crypted.push_back(grid2.at(coords_grid2.y, random_index));
    }

    return crypted;
}

ClassicalType ThreeSquares::decode(const ClassicalType &cipher_text)
{
    const uint32_t cipher_len = cipher_text.length();
    ClassicalType decrypted(2 * cipher_len / 3);

    srand(time(0));

    for (uint32_t i = 0; i < cipher_len; i += 3)
    {
        const auto coords_grid1 = grid.getCharCoordinates(cipher_text[i]);
        const auto coords_grid2 = grid2.getCharCoordinates(cipher_text[i + 2]);
        const auto coords_grid3 = grid3.getCharCoordinates(cipher_text[i + 1]);

        decrypted.push_back(grid.at(coords_grid3.y, coords_grid1.y));
        decrypted.push_back(grid2.at(coords_grid2.x, coords_grid3.x));
    }

    return decrypted;
}