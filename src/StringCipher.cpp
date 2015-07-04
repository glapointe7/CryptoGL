#include "StringCipher.hpp"

#include <algorithm>

#include "exceptions/EmptyAlpha.hpp"
#include "exceptions/MultipleChar.hpp"

using namespace CryptoGL;

void StringCipher::checkAlpha(const ClassicalType &alpha)
{
    if (alpha.empty())
    {
        throw EmptyAlpha("Your alphabet is empty.");
    }

    if (!alpha.containsUniqueChars())
    {
        throw MultipleChar("Your alphabet has to contain unique characters.");
    }
}

void StringCipher::setAlpha(const ClassicalType &alpha)
{
    checkAlpha(alpha);
    this->alpha = alpha;
}

ClassicalType StringCipher::getAlpha() const
{
    return alpha;
}

ClassicalType StringCipher::appendChars(ClassicalType data, const uint32_t to_add, const char c)
{
    const uint32_t rest = data.length() % to_add;
    if (rest != 0)
    {
        data.append(to_add - rest, c);
    }

    return data;
}

int8_t StringCipher::badAlphaFound(const ClassicalType &text) const
{
    for (const auto c : text)
    {
        if (alpha.find(c) == ClassicalType::npos)
        {
            return c;
        }
    }

    return 0;
}