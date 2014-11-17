
#ifndef SHA512TEST_HPP
#define SHA512TEST_HPP

#include "Test.hpp"
#include "../src/SHA2.hpp"

namespace UnitTests
{
    class SHA512Test : public Test
    {
    protected:
       SHA512 *S;

       void setUp() override
       {
          S = new SHA512();
       }

       void tearDown() override
       {
          delete S;
       }
    };

    TEST(SHA512EncodeShortText, SHA512Test)
    {
       const StringTest clear_text = "abc";
       const StringTest hash = "DDAF35A193617ABACC417349AE20413112E6FA4E89A97EA20A9EEEE64B55D39A2"
               "192992A274FC1A836BA3C23A3FEEBBD454D4423643CE80E2A9AC94FA54CA49F";

       compare(hash, S->encode(clear_text.toBytes()).toHexString());
    }

    TEST(SHA512EncodeLongText, SHA512Test)
    {
       const StringTest clear_text = "The quick brown fox jumps over the lazy dog";
       const StringTest hash = "07E547D9586F6A73F73FBAC0435ED76951218FB7D0C8D788A309D785436BBB642"
               "E93A252A954F23912547D1E8A3B5ED6E1BFD7097821233FA0538F3DB854FEE6";

       compare(hash, S->encode(clear_text.toBytes()).toHexString());
    }

    TEST(SHA512EncodeEmptyText, SHA512Test)
    {
       const StringTest hash = "CF83E1357EEFB8BDF1542850D66D8007D620E4050B5715DC83F4A921D36C"
               "E9CE47D0D13C5D85F2B0FF8318D2877EEC2F63B931BD47417A81A538327AF927DA3E";

       compare(hash, S->encode(BytesVector()).toHexString());
    }

    TEST(SHA512EncodeHMACShortText, SHA512Test)
    {
       const StringTest clear_text = "Hi There";
       const BytesVector key = {0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 
          0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B};
       const StringTest hash = "87AA7CDEA5EF619D4FF0B4241A1D6CB02379F4E2CE4EC2787AD0B30545E1"
          "7CDEDAA833B7D6B8A702038B274EAEA3F4E4BE9D914EEB61F1702E696C203A126854";

       compare(hash, S->hmacEncode(key, clear_text.toBytes()).toHexString());
    }
}

#endif