
#ifndef XTEATEST_HPP
#define XTEATEST_HPP

#include "Test.hpp"
#include "../src/XTEA.hpp"

namespace UnitTests
{
    class XTEATest : public Test
    {
    protected:
       XTEA *R, *C;

       void setUp() override
       {
          R = new XTEA(BytesVector(16, 0));
          C = new XTEA({0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef});
       }

       void tearDown() override
       {
          delete R;
          delete C;
       }
    };

    TEST(XTEAEncodeWith8Zero, XTEATest)
    {   
       compare(StringTest("DEE9D4D8F7131ED9"), R->encode(BytesVector(8, 0)).toHexString());
    }

    TEST(XTEADecodeWith8Zero, XTEATest)
    {
       compare(StringTest("0000000000000000"), R->decode(StringTest("DEE9D4D8F7131ED9").hexToBytes()).toHexString());
    }

    TEST(XTEAEncodeNormalCase, XTEATest)
    {
       const BytesVector clear_text = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
       compare(StringTest("27E795E076B2B537"), C->encode(clear_text).toHexString());
    }

    TEST(XTEADecodeNormalCase, XTEATest)
    {
       compare(StringTest("0123456789ABCDEF"), C->decode(StringTest("27E795E076B2B537").hexToBytes()).toHexString());
    }
}

#endif