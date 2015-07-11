#include "Delastelle.hpp"

#include "MathematicalTools.hpp"

using namespace CryptoGL;

void Delastelle::setBlockLength(const uint32_t block_len)
{
    if (block_len == 0)
    {
        throw ZeroBlockLength("The block length you provided has to be greater than zero.");
    }

    this->block_len = block_len;
}

ClassicalType Delastelle::encode(const ClassicalType &clear_text)
{
    const ClassicalType full_text = appendChars(clear_text, block_len, 'X');
    const uint32_t clear_len = full_text.length();

    // Separate the text in blocks of length chosen by block_len.
    const uint32_t block_size = clear_len / block_len;
    Grid block;
    block.reserve(block_size);
    for (uint32_t i = 0; i < clear_len; i += block_len)
    {
        block.push_back(ClassicalType(full_text.substr(i, block_len)));
    }

    // Under each letter, we note coordinates of letters vertically.
    ClassicalType crypted(clear_len);
    for (const auto &str : block)
    {
        BytesVector X, Y;
        X.reserve(block_len);
        Y.reserve(block_len);
        for (const auto c : str)
        {
            const Coordinates coords = grid.getCharCoordinates(c);
            X.push_back(coords.x);
            Y.push_back(coords.y);
        }

        if ((block_len % 2) == 0)
        {
            // Y coordinates.
            for (uint32_t i = 0; i < block_len; i += 2)
            {
                crypted.push_back(grid.at(Y[i], Y[i + 1]));
            }

            // X coordinates.
            for (uint32_t i = 0; i < block_len; i += 2)
            {
                crypted.push_back(grid.at(X[i], X[i + 1]));
            }
        }
        else
        {
            for (uint32_t i = 0; i < block_len - 1; i += 2)
            {
                crypted.push_back(grid.at(Y[i], Y[i + 1]));
            }
            crypted.push_back(grid.at(Y[block_len - 1], X[0]));
            for (uint32_t i = 1; i < block_len; i += 2)
            {
                crypted.push_back(grid.at(X[i], X[i + 1]));
            }
        }
    }

    return crypted;
}

ClassicalType Delastelle::decode(const ClassicalType &cipher_text)
{
    const uint32_t cipher_len = cipher_text.length() * 2;
    BytesVector chars_coords;
    chars_coords.reserve(cipher_len);
    for (const auto c : cipher_text)
    {
        const auto coords = grid.getCharCoordinates(c);
        chars_coords.push_back(coords.y);
        chars_coords.push_back(coords.x);
    }

    ClassicalType decrypted(cipher_len / 2);
    const uint32_t double_block_len = block_len * 2;
    for (uint32_t i = 0; i < cipher_len; i += double_block_len)
    {
        for (uint32_t k = 0; k < block_len; ++k)
        {
            decrypted.push_back(grid.at(chars_coords[i + k], chars_coords[i + k + block_len]));
        }
    }

    return decrypted;
}