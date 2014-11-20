#ifndef SHA1TEST_HPP
#define SHA1TEST_HPP

#include "Test.hpp"
#include "../src/SHA1.hpp"

namespace UnitTests
{
    class SHA1Test : public Test
    {
    protected:
       SHA1 *S;

       void setUp() override
       {
          S = new SHA1();
       }

       void tearDown() override
       {
          delete S;
       }
    };

    TEST(SHA1Encode, SHA1Test)
    {  
       compare("DA39A3EE5E6B4B0D3255BFEF95601890AFD80709", S->encode(BytesVector()));
    }

    TEST(SHA1EncodeHMACEmptyText, SHA1Test)
    {
       compare("FBDB1D1B18AA6C08324B7D64B71FB76370690E1D", S->hmacEncode(BytesVector(), BytesVector()));
    }

    TEST(SHA1EncodeHMACNormalCaseText, SHA1Test)
    {
       const StringTest clear_text = "The quick brown fox jumps over the lazy dog";
       const StringTest hash = "DE7C9B85B8B78AA6BC8A7A36F70A90701C9DB4D9";
       const StringTest hmac_key = "key";

       compare(hash, S->hmacEncode(hmac_key.toBytes(), 
               clear_text.toBytes()));
    }
}

#endif