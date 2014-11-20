
#ifndef SHA512_224TEST_HPP
#define SHA512_224TEST_HPP

#include "Test.hpp"
#include "../src/SHA2.hpp"

namespace UnitTests
{
    class SHA512_224Test : public Test
    {
    protected:
       SHA512_224 *S;

       void setUp() override
       {
          S = new SHA512_224();
       }

       void tearDown() override
       {
          delete S;
       }
    };

    TEST(SHA512_224EncodeNormalCaseText, SHA512_224Test)
    {
       const StringTest clear_text = "The quick brown fox jumps over the lazy dog";
       const StringTest hash = "944CD2847FB54558D4775DB0485A50003111C8E5DAA63FE722C6AA37";

       compare(hash, S->encode(clear_text.toBytes()));
    }

    TEST(SHA512_224EncodeEmptyText, SHA512_224Test)
    {
       const StringTest hash = "6ED0DD02806FA89E25DE060C19D3AC86CABB87D6A0DDD05C333B84F4";

       compare(hash, S->encode({}));
    }
}

#endif