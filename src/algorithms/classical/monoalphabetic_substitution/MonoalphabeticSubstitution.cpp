#include "MonoalphabeticSubstitution.hpp"

using namespace CryptoGL;

ClassicalType MonoalphabeticSubstitution::encode(const ClassicalType &clear_text)
{
    const KeyType key = getKey();
    ClassicalType crypted;

    for (const auto c : clear_text)
    {
        crypted = crypted + key[alpha.find(c)];
    }

    return crypted;
}

ClassicalType MonoalphabeticSubstitution::decode(const ClassicalType &cipher_text)
{
    const KeyType key = getKey();
    ClassicalType decrypted;

    for (const auto c : cipher_text)
    {
        decrypted = decrypted + alpha[key.find(c)];
    }

    return decrypted;
}