
#ifndef SHA256TEST_HPP
#define SHA256TEST_HPP

#include "Test.hpp"
#include "../src/SHA2.hpp"

namespace UnitTests
{
    class SHA256Test : public Test
    {
    protected:
       SHA256 *S;

       void setUp() override
       {
          S = new SHA256();
       }

       void tearDown() override
       {
          delete S;
       }
    };

    TEST(SHA256EncodeNormalCaseText, SHA256Test)
    {
       const StringTest clear_text = "The quick brown fox jumps over the lazy dog";
       const StringTest hash = "D7A8FBB307D7809469CA9ABCB0082E4F8D5651E46D3CDB762D02D0BF37C9E592";

       compare(hash, S->encode(clear_text.toBytes()).toHexString());
    }

    TEST(SHA256EncodeEmptyText, SHA256Test)
    {
       const StringTest hash = "E3B0C44298FC1C149AFBF4C8996FB92427AE41E4649B934CA495991B7852B855";

       compare(hash, S->encode(BytesVector()).toHexString());
    }

    TEST(SHA256EncodeHMACNormalCaseText, SHA256Test)
    {
       const StringTest clear_text = "Hi There";
       const StringTest hash = "B0344C61D8DB38535CA8AFCEAF0BF12B881DC200C9833DA726E9376C2E32CFF7";

       compare(hash, S->hmacEncode(
          {0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B}, 
               clear_text.toBytes()).toHexString());
    }

    TEST(SHA256EncodeHMACEmptyText, SHA256Test)
    {
       const StringTest hash = "B613679A0814D9EC772F95D778C35FC5FF1697C493715653C6C712144292C5AD";

       compare(hash, S->hmacEncode(BytesVector(), BytesVector()).toHexString());
    }
}

#endif