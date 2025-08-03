#pragma once

#include "Types.hpp"

namespace CryptoGL
{
    namespace Tools
    {
        /* Convert uppercase to symbol[0] and lowercase to symbol[1]. */
        void convertMajMinToSymbol(String &text, const String symbol);

        String convertFromBaseXToBaseY(const String &number, const uint8_t from_base, const uint8_t to_base);
    }
}