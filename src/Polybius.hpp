
#ifndef POLYBIUS_HPP
#define POLYBIUS_HPP

#include "SquareCipher.hpp"

namespace CryptoGL
{
    class Polybius : public SquareCipher
    {
    public:
        Polybius(const KeyType &key, const ClassicalType &alpha) : SquareCipher(key, alpha) { }
        
        explicit Polybius(const KeyType &key) : Polybius(key, ClassicalType::GRID_UPPERCASE_FR) { }

        ClassicalType encode(const ClassicalType &clear_text) override;
        ClassicalType decode(const ClassicalType &cipher_text) override;
    };
}

#endif