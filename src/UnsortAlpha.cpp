#include "UnsortAlpha.hpp"

#include "String.hpp"

using namespace CryptoGL;

UnsortAlpha::UnsortAlpha(const KeyType &key)
    : charEncode(sortAlpha), charDecode(unorderedAlpha), unsort_alpha(alpha.length())
{
    setKey(key);
}

void UnsortAlpha::setHorizontalAlpha()
{
    ClassicalType key_alpha(getKey().append(alpha));
    key_alpha.removeDuplicates();
    unsort_alpha = key_alpha;
}

// Build an unordered vertical alphabet by transforming the key with only unique chars.
void UnsortAlpha::setVerticalAlpha()
{
    ClassicalType new_key(getKey());
    new_key.removeDuplicates();
    const uint32_t key_len = new_key.length();

    ClassicalType str(new_key.append(alpha));
    str.removeDuplicates();
    const uint32_t alpha_len = str.length();

    for (uint32_t i = 0; i < key_len; ++i)
    {
        uint32_t j = i;
        while (j < alpha_len)
        {
            unsort_alpha.push_back(str[j]);
            j += key_len;
        }
    }
}

ClassicalType UnsortAlpha::process(const ClassicalType &text, const GetCharFunction &getNextChar)
{
    ClassicalType output;
    output.reserve(text.length());
    for (const auto c : text)
    {
        output.push_back(getNextChar(alpha, unsort_alpha, c));
    }

    return output;
}

ClassicalType UnsortAlpha::encode(const ClassicalType &clear_text)
{
    return process(clear_text, charEncode);
}

ClassicalType UnsortAlpha::decode(const ClassicalType &cipher_text)
{
    return process(cipher_text, charDecode);
}