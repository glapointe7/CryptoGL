#pragma once

#include "Test.hpp"
#include "../src/RC5.hpp"

namespace UnitTests
{
    class RC5Test : public Test
    {
    protected:
       RC5 *R, *C;

       void setUp() override
       {
          R = new RC5(BytesVector(16, 0));
          C = new RC5({0x91, 0x5F, 0x46, 0x19, 0xBE, 0x41, 0xB2, 0x51, 0x63, 0x55, 0xA5, 0x01, 0x10, 0xA9, 0xCE, 0x91});
       }

       void tearDown() override
       {
          delete R;
          delete C;
       }
    };

    TEST(RC5EncodeZeroValues, RC5Test)
    {   
       compare("21A5DBEE154B8F6D", R->encode(BytesVector(8, 0)));
    }

    TEST(RC5DecodeZeroValues, RC5Test)
    {
       compare("0000000000000000", R->decode(StringTest("21A5DBEE154B8F6D").hexToBytes()));
    }

    TEST(RC5EncodeNormalCase, RC5Test)
    {
       const BytesVector clear_text = {0x21, 0xA5, 0xDB, 0xEE, 0x15, 0x4B, 0x8F, 0x6D};
       compare("F7C013AC5B2B8952", C->encode(clear_text));
    }

    TEST(RC5DecodeNormalCase, RC5Test)
    {
       compare("21A5DBEE154B8F6D", C->decode(StringTest("F7C013AC5B2B8952").hexToBytes()));
    }
}