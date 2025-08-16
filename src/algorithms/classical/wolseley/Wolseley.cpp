#include "Wolseley.hpp"

using namespace CryptoGL;

Wolseley::Wolseley(const KeyType &key)
{
    setAlpha(ClassicalType::grid_uppercase_fr);
    setKey(key);
}

ClassicalType Wolseley::encode(const ClassicalType &clear_text)
{
    ClassicalType new_alpha(getKey().append(alpha));
    new_alpha.removeDuplicates();
    ClassicalType crypted(clear_text.length());
    const uint64_t new_alpha_length = new_alpha.length();
    for (const auto c : clear_text)
    {
        const uint8_t pos = new_alpha_length - new_alpha.find(c) - 1;
        crypted.push_back(new_alpha[pos]);
    }

    return crypted;
}

ClassicalType Wolseley::decode(const ClassicalType &cipher_text)
{
    return encode(cipher_text);
}