
#ifndef TWOSQUARES_HPP
#define TWOSQUARES_HPP

#include "SquareCipher.hpp"
#include "CipherGrid.hpp"

namespace CryptoGL
{
    class TwoSquares : public SquareCipher
    {
    public:
        TwoSquares(const KeyType &key1, const KeyType &key2, const ClassicalType &alpha);
        TwoSquares(const KeyType &key1, const KeyType &key2)
            : TwoSquares(key1, key2, ClassicalType::GRID_UPPERCASE_FR) { }

        ClassicalType encode(const ClassicalType &clear_text) override;
        ClassicalType decode(const ClassicalType &cipher_text) override;

    private:
        KeyType key2;

        const CipherGrid grid2;
    };
}

#endif