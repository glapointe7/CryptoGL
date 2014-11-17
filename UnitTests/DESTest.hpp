#ifndef DESTEST_HPP
#define DESTEST_HPP

#include "Test.hpp"
#include "../src/DES.hpp"

namespace UnitTests
{
    class DESTest : public Test
    {
    protected:
       DES *D;

       void setUp() override
       {
          D = new DES({0x13, 0x34, 0x57, 0x79, 0x9B, 0xBC, 0xDF, 0xF1});
       }

       void tearDown() override
       {
          delete D;
       }
    };

    TEST(DESEncode, DESTest)
    {
       const BytesVector clear_text = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};

       compare(StringTest("85E813540F0AB405"), D->encode(clear_text).toHexString());
    }

    TEST(DESDecode, DESTest)
    {
       const BytesVector clear_text = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
       const StringTest message("85E813540F0AB405");

       compare(clear_text.toHexString(), D->decode(message.hexToBytes()).toHexString());
    }
}

#endif