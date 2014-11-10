
#ifndef STRINGCIPHER_HPP
#define STRINGCIPHER_HPP

#include "Cipher.hpp"
#include "String.hpp"

namespace CryptoGL
{
    class StringCipher : public Cipher<ClassicalType>
    {
    public:
       virtual void setAlpha(const ClassicalType &alpha);
       ClassicalType getAlpha() const;

    protected: 
       StringCipher() : alpha(ClassicalType::uppercase) {}
       virtual ~StringCipher() {}

       virtual ClassicalType encode(const ClassicalType &) = 0;
       virtual ClassicalType decode(const ClassicalType &) = 0;

       int8_t badAlphaFound(const ClassicalType &text) const;
       static ClassicalType appendChars(ClassicalType data, const uint32_t to_add, const char c);
       static void checkAlpha(const ClassicalType &alpha);

       ClassicalType alpha;
    };
}

#endif