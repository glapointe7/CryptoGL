
#ifndef AFFINE_HPP
#define AFFINE_HPP

#include "StringCipher.hpp"

namespace CryptoGL
{

    class Affine : public StringCipher
    {
    public:

        Affine(const int32_t a, const int32_t b)
        {
            setKey(a, b);
        }

        /* Encode with the formula : y = (ax + b) (mod alpha_length). */
        ClassicalType encode(const ClassicalType &clear_text) override;

        /* Decode with the formula : y = a^-1(y - b) (mod alpha_length). */
        ClassicalType decode(const ClassicalType &cipher_text) override;

        void setKey(const int32_t a, const int32_t b);

    private:
        int32_t a;
        int32_t b;
    };
}

#endif