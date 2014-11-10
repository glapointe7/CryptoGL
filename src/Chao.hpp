
#ifndef CHAO_HPP
#define CHAO_HPP

#include "StringCipher.hpp"

namespace CryptoGL
{
    class Chao : public StringCipher
    {
    public:
       Chao(const ClassicalType &left_alpha, const ClassicalType &right_alpha);

       ClassicalType encode(const ClassicalType &clear_text) override;
       ClassicalType decode(const ClassicalType &cipher_text) override;

    private:
       ClassicalType left_alpha;
       ClassicalType right_alpha;
       static constexpr uint8_t nadir = 13;

       void leftShiftAlphabet(const uint8_t index);
       void rightShiftAlphabet(const uint8_t index);
    };
}

#endif