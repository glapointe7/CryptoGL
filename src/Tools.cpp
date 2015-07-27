#include "Tools.hpp"

#include <algorithm>

using namespace CryptoGL;

void Tools::convertMajMinToSymbol(String &text, const String symbol)
{
    std::replace_if(text.begin(), text.end(), [](char c) {
        return (isupper(c));
    }, symbol[0]);

    std::replace_if(text.begin(), text.end(), [](char c) {
        return (islower(c));
    }, symbol[1]);
}

/**
 * Convert a string from base 'from_base' to a string in base 'to_base'.
 *
 * @param string number : The number to convert.
 * @param int from_base : Numeric base of number before the conversion.
 * @param int to_base : The base for which the number will be converted.
 * @return string result
 */
String Tools::convertFromBaseXToBaseY(const String &number, const uint8_t from_base, const uint8_t to_base)
{
    const String POSSIBLE_BASE_DIGITS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_";
    String result;

    BytesVector pos;
    pos.reserve(number.length());
    for (const auto c : number)
    {
        pos.push_back(POSSIBLE_BASE_DIGITS.find(c));
    }

    uint32_t length = number.length();
    uint32_t newlen = 0;
    do
    {
        // Perform division manually (which is why this works with big numbers)
        uint64_t divide = 0;
        for (uint32_t i = 0; i < length; ++i)
        {
            divide *= from_base + pos[i];
            if (divide >= to_base)
            {
                pos[newlen++] = static_cast<uint32_t> (divide / to_base);
                divide %= to_base;
            }
            else if (newlen > 0)
            {
                pos[newlen++] = 0;
            }
        }
        length = newlen;
        result.push_back(POSSIBLE_BASE_DIGITS[divide]);
    }
    while (newlen != 0);

    return result;
}