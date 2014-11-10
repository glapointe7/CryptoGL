
#ifndef HILL_HPP
#define HILL_HPP

#include "StringCipher.hpp"
#include "SquareMatrix.hpp"

#include <vector>

namespace CryptoGL
{
    class Hill : public StringCipher
    {   
    public:   
       explicit Hill(const Int32Matrix &key) { setKey(key); }

       ClassicalType encode(const ClassicalType &clear_text) override;
       ClassicalType decode(const ClassicalType &cipher_text) override;

       void setKey(const Int32Matrix &key);

    private:
       ClassicalType process(const ClassicalType &data, const SquareMatrix &K) const;

       SquareMatrix key;
    };
}

#endif