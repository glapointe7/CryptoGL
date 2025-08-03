/*
 * Source : http://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64#C.2B.2B
 */
#pragma once

#include "Cipher.hpp"

namespace CryptoGL
{

    class Base64 : public Cipher<ClassicalType, BytesVector>
    {
    public:
        ClassicalType encode(const BytesVector &clear_data) override;
        BytesVector decode(const ClassicalType &cipher_data) override;

    private:
        static constexpr uint8_t pad_character = '=';
        
        static uint8_t countPaddingCharacters(const ClassicalType &cipher_data);
    };
}