
#ifndef HILL_HPP
#define HILL_HPP

#include "StringCipher.hpp"
#include "SquareMatrix.hpp"

namespace CryptoGL
{

    class Hill : public StringCipher
    {
    public:
        explicit Hill(const Int64Matrix &key)
        {
            setKey(key);
        }

        ClassicalType encode(const ClassicalType &clear_text) override;
        ClassicalType decode(const ClassicalType &cipher_text) override;

        void setKey(const Int64Matrix &key);

    private:
        /* Process encode / decode of the data with the matrix key K. */
        ClassicalType process(const ClassicalType &data) const;

        SquareMatrix key;
    };
}

#endif