
#ifndef TWOFISHTEST_HPP
#define TWOFISHTEST_HPP

#include "Test.hpp"
#include "../src/Twofish.hpp"

namespace UnitTests
{
    class TwofishTest : public Test
    {
    protected:
       Twofish *C128, *C192, *C256;

       void setUp() override
       {
          C128 = new Twofish(BytesVector(16, 0));
          C192 = new Twofish({0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10, 
                  0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77});
          C256 = new Twofish({0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10, 
                  0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF});
       }

       void tearDown() override
       {
          delete C128;
          delete C192;
          delete C256;
       }
    };

    TEST(TwofishEncode128Bits, TwofishTest)
    {
       compare("9F589F5CF6122C32B6BFEC2F2AE8C35A", C128->encode(BytesVector(16, 0)).toHexString());
    }

    TEST(TwofishDecode128Bits, TwofishTest)
    {
       const StringTest code = "9F589F5CF6122C32B6BFEC2F2AE8C35A";
       compare("00000000000000000000000000000000", C128->decode(code.hexToBytes()).toHexString());
    }

    TEST(TwofishEncode192Bits, TwofishTest)
    {
       compare("CFD1D2E5A9BE9CDF501F13B892BD2248", C192->encode(BytesVector(16, 0)).toHexString());
    }

    TEST(TwofishDecode192Bits, TwofishTest)
    {
       const StringTest code = "CFD1D2E5A9BE9CDF501F13B892BD2248";
       compare("00000000000000000000000000000000", C192->decode(code.hexToBytes()).toHexString());
    }

    TEST(TwofishEncode256Bits, TwofishTest)
    {
       compare("37527BE0052334B89F0CFCCAE87CFA20", C256->encode(BytesVector(16, 0)).toHexString());
    }

    TEST(TwofishDecode256Bits, TwofishTest)
    {
       const StringTest code = "37527BE0052334B89F0CFCCAE87CFA20";
       compare("00000000000000000000000000000000", C256->decode(code.hexToBytes()).toHexString());
    }
}

#endif