/*
 * Source : http://tools.ietf.org/html/rfc2144
 */
#ifndef CAST128TEST_HPP
#define CAST128TEST_HPP

#include "Test.hpp"
#include "../src/CAST128.hpp"

namespace UnitTests
{
    class CAST128Test : public Test
    {
    protected:
       CAST128 *C128, *C80, *C40;

       void setUp() override
       {
          C128 = new CAST128({0x01, 0x23, 0x45, 0x67, 0x12, 0x34, 0x56, 0x78, 0x23, 0x45, 0x67, 0x89, 0x34, 0x56, 0x78, 0x9A});
          C80 = new CAST128({0x01, 0x23, 0x45, 0x67, 0x12, 0x34, 0x56, 0x78, 0x23, 0x45});
          C40 = new CAST128({0x01, 0x23, 0x45, 0x67, 0x12});
       }

       void tearDown() override
       {
          delete C128;
          delete C80;
          delete C40;
       }
    };

    TEST(CAST128Encode128Bits, CAST128Test)
    {
       const BytesVector clear_text = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};

       compare("238B4FE5847E44B2", C128->encode(clear_text).toHexString());
    }

    TEST(CAST128Decode128Bits, CAST128Test)
    {
       const StringTest code = "238B4FE5847E44B2";
       compare("0123456789ABCDEF", C128->decode(code.hexToBytes()).toHexString());
    }

    TEST(CAST128Encode80Bits, CAST128Test)
    {
       const BytesVector clear_text = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};

       compare("EB6A711A2C02271B", C80->encode(clear_text).toHexString());
    }

    TEST(CAST128Decode80Bits, CAST128Test)
    {
       const StringTest code = "EB6A711A2C02271B";
       compare("0123456789ABCDEF", C80->decode(code.hexToBytes()).toHexString());
    }

    TEST(CAST128Encode40Bits, CAST128Test)
    {
       const BytesVector clear_text = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};

       compare("7AC816D16E9B302E", C40->encode(clear_text).toHexString());
    }

    TEST(CAST128Decode40Bits, CAST128Test)
    {
       const StringTest code = "7AC816D16E9B302E";
       compare("0123456789ABCDEF", C40->decode(code.hexToBytes()).toHexString());
    }
}

#endif