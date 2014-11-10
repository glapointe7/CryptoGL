
#ifndef MONOALPHABETICSUBSTITUTION_HPP
#define MONOALPHABETICSUBSTITUTION_HPP

#include "StringCipherWithStringKey.hpp"

namespace CryptoGL
{
    class MonoalphabeticSubstitution : public StringCipherWithStringKey
    {
    public:
       explicit MonoalphabeticSubstitution(const KeyType &key) {setKey(key); }

       ClassicalType encode(const ClassicalType &clear_text) override;
       ClassicalType decode(const ClassicalType &cipher_text) override;
    };
}

#endif