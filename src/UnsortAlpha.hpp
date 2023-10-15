
#ifndef UNSORTALPHA_HPP
#define UNSORTALPHA_HPP

#include "StringCipherWithStringKey.hpp"

#include <functional>

namespace CryptoGL
{
    class UnsortAlpha : public StringCipherWithStringKey
    {
    public:
        explicit UnsortAlpha(const KeyType &key);

        ClassicalType encode(const ClassicalType &clear_text) override;
        ClassicalType decode(const ClassicalType &cipher_text) override;

        /* Unordered the alphabet in the vertical way. */
        void setVerticalAlpha();

        /* Unordered the alphabet in the horizontal way. */
        void setHorizontalAlpha();

    private:
        using GetCharFunction = std::function<char(const ClassicalType &, const ClassicalType &, const char) >;

        const GetCharFunction charEncode, charDecode;
        ClassicalType unsort_alpha;
        
        static char sortAlpha(const ClassicalType &alpha, const ClassicalType &unsort_alpha, const char c)
        {
            return unsort_alpha[alpha.find(c)];
        }
        
        static char unorderedAlpha(const ClassicalType &alpha, const ClassicalType &unsort_alpha, const char c)
        {
            return alpha[unsort_alpha.find(c)];
        }

        ClassicalType process(const ClassicalType &text, const GetCharFunction &getNextChar);
    };
}

#endif