/*
 * Tests from (http://tools.ietf.org/html/rfc1321)
 */
#ifndef MD5TEST_HPP
#define MD5TEST_HPP

#include "Test.hpp"
#include "../src/MessageDigest.hpp"

namespace UnitTests
{
    class MD5Test : public Test
    {
    protected:
       MD5 *S;

       void setUp() override
       {
          S = new MD5();
       }

       void tearDown() override
       {
          delete S;
       }
    };

    TEST(MD5EncodeNormalCaseText, MD5Test)
    {
       const StringTest clear_text = "The quick brown fox jumps over the lazy dog";
       const StringTest hash = "9E107D9D372BB6826BD81D3542A419D6";

       compare(hash, S->encode(clear_text.toBytes()));
    }

    TEST(MD5EncodeEmptyText, MD5Test)
    {
       const StringTest hash = "D41D8CD98F00B204E9800998ECF8427E";

       compare(hash, S->encode({}));
    }

    TEST(MD5EncodeHMACNormalCaseText, MD5Test)
    {
       const StringTest clear_text = "The quick brown fox jumps over the lazy dog";
       const StringTest hash = "80070713463E7749B90C2DC24911E275";

       compare(hash, S->hmacEncode(StringTest("key").toBytes(), clear_text.toBytes()));
    }

    TEST(MD5EncodeHMACEmptyText, MD5Test)
    {
       const StringTest hash = "74E6F7298A9C2D168935F58C001BAD88";

       compare(hash, S->hmacEncode({}, {}));
    }
}

#endif