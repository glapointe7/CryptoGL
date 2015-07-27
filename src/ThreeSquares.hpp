#ifndef THREESQUARES_HPP
#define THREESQUARES_HPP

#include "SquareCipher.hpp"
#include "CipherGrid.hpp"

namespace CryptoGL
{
    class ThreeSquares : public SquareCipher
    {
    public:
        ThreeSquares(const KeyType &key1, const KeyType &key2, const KeyType &key3, const ClassicalType &alpha);
        ThreeSquares(const KeyType &key1, const KeyType &key2, const KeyType &key3)
            : ThreeSquares(key1, key2, key3, ClassicalType::GRID_UPPERCASE_FR) { }

        ClassicalType encode(const ClassicalType &clear_text) override;
        ClassicalType decode(const ClassicalType &cipher_text) override;

    private:
        KeyType key2;
        KeyType key3;

        const CipherGrid grid2, grid3;
    };
}

#endif