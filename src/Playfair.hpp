
#ifndef PLAYFAIR_HPP
#define PLAYFAIR_HPP

#include "SquareCipher.hpp"

namespace CryptoGL
{
    class Playfair : public SquareCipher
    {
    public:
        Playfair(const KeyType &key, const ClassicalType &alpha) : SquareCipher(key, alpha) { }
        explicit Playfair(const KeyType &key) : Playfair(key, ClassicalType::grid_uppercase_fr) { }

        ClassicalType encode(const ClassicalType &clear_text) override;
        ClassicalType decode(const ClassicalType &cipher_text) override;
    };
}

#endif