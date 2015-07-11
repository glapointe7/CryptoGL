#include "Chao.hpp"

#include "exceptions/BadAlphaLength.hpp"

#include <algorithm>

using namespace CryptoGL;

Chao::Chao(const ClassicalType &left_alpha, const ClassicalType &right_alpha)
{
    checkAlpha(left_alpha);
    checkAlpha(right_alpha);
    if (left_alpha.length() != right_alpha.length())
    {
        throw BadAlphaLength("Chao constructor: Your left alphabet has to have the same length as the right one.", left_alpha.length());
    }

    this->left_alpha = left_alpha;
    this->right_alpha = right_alpha;
}

void Chao::leftShiftAlphabet(const uint8_t index)
{
    left_alpha.rotateLeft(index);
    const char tmp = left_alpha[1];
    left_alpha.erase(left_alpha.begin() + 1);
    left_alpha.insert(left_alpha.begin() + nadir, tmp);
}

void Chao::rightShiftAlphabet(const uint8_t index)
{
    right_alpha.rotateLeft(index + 1);
    const char tmp = right_alpha[2];
    right_alpha.erase(right_alpha.begin() + 2);
    right_alpha.insert(right_alpha.begin() + nadir, tmp);
}

ClassicalType Chao::encode(const ClassicalType &clear_text)
{
    ClassicalType crypted;
    crypted.reserve(clear_text.length());

    for (const auto c : clear_text)
    {
        const uint8_t index = right_alpha.find(c);
        const int8_t corresponding_char = left_alpha[index];
        crypted.push_back(corresponding_char);
        leftShiftAlphabet(index);
        rightShiftAlphabet(index);
    }

    return crypted;
}

ClassicalType Chao::decode(const ClassicalType &cipher_text)
{
    ClassicalType decrypted;
    decrypted.reserve(cipher_text.length());

    for (const auto c : cipher_text)
    {
        const uint8_t index = left_alpha.find(c);
        const int8_t corresponding_char = right_alpha[index];
        decrypted.push_back(corresponding_char);
        leftShiftAlphabet(index);
        rightShiftAlphabet(index);
    }

    return decrypted;
}