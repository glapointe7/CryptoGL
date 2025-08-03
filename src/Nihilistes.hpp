#pragma once

#include "SquareCipher.hpp"

namespace CryptoGL
{
    class Nihilistes : public SquareCipher
    {
    public:
        Nihilistes(const KeyType &key, const KeyType &second_key, const ClassicalType &alpha);
        Nihilistes(const KeyType &key, const KeyType &second_key)
            : Nihilistes(key, second_key, ClassicalType::grid_uppercase_fr) { }

        ClassicalType encode(const ClassicalType &clear_text) override;
        ClassicalType decode(const ClassicalType &cipher_text) override;

    private:
        KeyType second_key;
    };
}