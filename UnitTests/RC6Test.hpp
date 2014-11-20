
#ifndef RC6TEST_HPP
#define RC6TEST_HPP

#include "Test.hpp"
#include "../src/RC6.hpp"

namespace UnitTests
{
    class RC6Test : public Test
    {
    protected:
       RC6 *R, *C;

       void setUp() override
       {
          R = new RC6(BytesVector(16, 0));
          C = new RC6({0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01, 0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78});
       }

       void tearDown() override
       {
          delete R;
          delete C;
       }
    };

    TEST(RC6EncodeZeroValues, RC6Test)
    {   
       compare("8FC3A53656B1F778C129DF4E9848A41E", R->encode(BytesVector(16, 0)));
    }

    TEST(RC6DecodeZeroValues, RC6Test)
    {
       compare("00000000000000000000000000000000", R->decode(StringTest("8FC3A53656B1F778C129DF4E9848A41E").hexToBytes()));
    }

    TEST(RC6EncodeNormalCase, RC6Test)
    {
       const BytesVector clear_text = {0x02, 0x13, 0x24, 0x35, 0x46, 0x57, 0x68, 0x79, 0x8A, 0x9B, 0xAC, 0xBD, 0xCE, 0xDF, 0xE0, 0xF1};
       compare("524E192F4715C6231F51F6367EA43F18", C->encode(clear_text));
    }

    TEST(RC6DecodeNormalCase, RC6Test)
    {
       compare("02132435465768798A9BACBDCEDFE0F1", C->decode(StringTest("524E192F4715C6231F51F6367EA43F18").hexToBytes()));
    }
}

#endif