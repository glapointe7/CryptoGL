
#ifndef WOLSELEY_HPP
#define WOLSELEY_HPP

#include "StringCipherWithStringKey.hpp"

namespace CryptoGL
{
    class Wolseley : public StringCipherWithStringKey
    {
    public:
       explicit Wolseley(const KeyType &key);

       ClassicalType encode(const ClassicalType &clear_text) override;
       ClassicalType decode(const ClassicalType &cipher_text) override;
    };
}

#endif