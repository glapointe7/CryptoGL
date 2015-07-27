
#ifndef STRINGCIPHER_HPP
#define STRINGCIPHER_HPP

#include "Cipher.hpp"

namespace CryptoGL
{
    class StringCipher : public Cipher<ClassicalType>
    {
    public:
        void setAlpha(const ClassicalType &alpha);
        ClassicalType getAlpha() const;

    protected:
        StringCipher() : alpha(ClassicalType::UPPERCASE) { }

        virtual ClassicalType encode(const ClassicalType &) override = 0;
        virtual ClassicalType decode(const ClassicalType &) override = 0;

        /* Check if text has at least a character that doesn't belong to alpha.
         * Return 0 if nothing is found, otherwise it returns the first char found.*/
        int8_t badAlphaFound(const ClassicalType &text) const;

        /* Append the character c (text.length() % mod) times.*/
        static ClassicalType appendChars(ClassicalType data, const uint32_t to_add, const char c);
        static void checkAlpha(const ClassicalType &alpha);

        ClassicalType alpha;
    };
}

#endif