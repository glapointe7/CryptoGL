/*
 * Source : http://tools.ietf.org/html/rfc3713
 */
#ifndef CAMELLIATEST_HPP
#define CAMELLIATEST_HPP

#include "Test.hpp"
#include "../src/Camellia.hpp"

namespace UnitTests
{
    class CamelliaTest : public Test
    {
    protected:
       Camellia *C128, *C192, *C256;

       void setUp() override
       {
          C128 = new Camellia({0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10});
          C192 = new Camellia({0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10,
             0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77});
          C256 = new Camellia({0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10,
             0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF});
       }

       void tearDown() override
       {
          delete C128;
          delete C192;
          delete C256;
       }
    };

    TEST(CamelliaEncode128Bits, CamelliaTest)
    {
       const BytesVector clear_text = {
          0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10
       };

       compare("67673138549669730857065648EABE43", C128->encode(clear_text));
    }

    TEST(CamelliaDecode128Bits, CamelliaTest)
    {
       const StringTest code = "67673138549669730857065648EABE43";
       compare("0123456789ABCDEFFEDCBA9876543210", 
               C128->decode(code.hexToBytes()));
    }

    TEST(CamelliaEncode192Bits, CamelliaTest)
    {
       const BytesVector clear_text = {
          0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10
       };

       compare("B4993401B3E996F84EE5CEE7D79B09B9", C192->encode(clear_text));
    }

    TEST(CamelliaDecode192Bits, CamelliaTest)
    {
       StringTest code("B4993401B3E996F84EE5CEE7D79B09B9");
       compare("0123456789ABCDEFFEDCBA9876543210", 
               C192->decode(code.hexToBytes()));
    }

    TEST(CamelliaEncode256Bits, CamelliaTest)
    {
       const BytesVector clear_text = {
          0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10
       };

       compare("9ACC237DFF16D76C20EF7C919E3A7509", C256->encode(clear_text));
    }

    TEST(CamelliaDecode256Bits, CamelliaTest)
    {
       StringTest code("9ACC237DFF16D76C20EF7C919E3A7509");
       compare("0123456789ABCDEFFEDCBA9876543210", 
               C256->decode(code.hexToBytes()));
    }
}

#endif