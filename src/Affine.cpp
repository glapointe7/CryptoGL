#include "Affine.hpp"

#include "MathematicalTools.hpp"

using namespace CryptoGL;

void Affine::setKey(const int32_t a, const int32_t b)
{
    this->a = a;
    this->b = b;
}

ClassicalType Affine::encode(const ClassicalType &clear_text)
{
    const uint32_t alpha_length = alpha.length();
    ClassicalType crypted;
    crypted.reserve(clear_text.length());

    for (const auto c : clear_text)
    {
        const auto number = Maths::mod((a * alpha.find(c)) + b, alpha_length);
        crypted.push_back(alpha[number]);
    }

    return crypted;
}

ClassicalType Affine::decode(const ClassicalType &cipher_text)
{
    const int32_t alpha_length = alpha.length();
    ClassicalType decrypted;
    decrypted.reserve(cipher_text.length());
    const auto a_inv = Maths::getModInverse(a, alpha_length);

    for (const auto c : cipher_text)
    {
        const auto number = Maths::mod(a_inv * (static_cast<int8_t> ((alpha.find(c)) - b)), alpha_length);
        decrypted.push_back(alpha[number]);
    }

    return decrypted;
}