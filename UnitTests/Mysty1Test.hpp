/*
 * Source : http://www.ipa.go.jp/security/rfc/RFC2994EN.html
 */
#ifndef MYSTY1TEST_HPP
#define MYSTY1TEST_HPP

#include "Test.hpp"
#include "../src/Mysty1.hpp"

namespace UnitTests
{
    class Mysty1Test : public Test
    {
    protected:
       Mysty1 *ECB, *CBC;

       void setUp() override
       {
          ECB = new Mysty1({0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF});
          CBC = new Mysty1({0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF},
                         OperationModes::CBC,{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08});
       }

       void tearDown() override
       {
          delete ECB;
          delete CBC;
       }
    };

    TEST(Mysty1ECBEncode, Mysty1Test)
    {
       const BytesVector clear_text = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD,
          0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};

       compare(StringTest("8B1DA5F56AB3D07C04B68240B13BE95D"), ECB->encode(clear_text).toHexString());
    }

    TEST(Mysty1ECBDecode, Mysty1Test)
    {
       const BytesVector clear_text = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD,
          0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};

       const StringTest expected("8B1DA5F56AB3D07C04B68240B13BE95D");
       compare(clear_text.toHexString(), ECB->decode(expected.hexToBytes()).toHexString());
    }

    TEST(Mysty1CBCEncode, Mysty1Test)
    {
       const BytesVector clear_text = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD,
          0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};

       compare(StringTest("461C1E879C18C27FB9ADF2D80C89031F"), CBC->encode(clear_text).toHexString());
    }

    TEST(Mysty1CBCDecode, Mysty1Test)
    {
       const BytesVector clear_text = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD,
          0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};

       const StringTest expected("461C1E879C18C27FB9ADF2D80C89031F");
       compare(clear_text.toHexString(), CBC->decode(expected.hexToBytes()).toHexString());
    }
}

#endif