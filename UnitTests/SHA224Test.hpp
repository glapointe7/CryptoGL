
#ifndef SHA224TEST_HPP
#define SHA224TEST_HPP

#include "Test.hpp"
#include "../src/SHA2.hpp"

namespace UnitTests
{
    class SHA224Test : public Test
    {
    protected:
       SHA224 *S;

       void setUp() override
       {
          S = new SHA224();
       }

       void tearDown() override
       {
          delete S;
       }
    };

    TEST(SHA224EncodeNormalCaseText, SHA224Test)
    {
       const StringTest clear_text = "The quick brown fox jumps over the lazy dog";
       const StringTest hash = "730E109BD7A8A32B1CB9D9A09AA2325D2430587DDBC0C38BAD911525";

       compare(hash, S->encode(clear_text.toBytes()));
    }

    TEST(SHA224EncodeEmptyText, SHA224Test)
    {
       const StringTest hash = "D14A028C2A3A2BC9476102BB288234C415A2B01F828EA62AC5B3E42F";

       compare(hash, S->encode(BytesVector()));
    }

    TEST(SHA224EncodeHMACNormalCaseText, SHA224Test)
    {
       const StringTest clear_text = "Hi There";
       const BytesVector key = {0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 
          0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B};
       const StringTest hash = "896FB1128ABBDF196832107CD49DF33F47B4B1169912BA4F53684B22";

       compare(hash, S->hmacEncode(key, clear_text.toBytes()));
    }
}

#endif