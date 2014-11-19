/*
 * Test vectors from http://en.wikipedia.org/wiki/RC4#Test_vectors
 * http://tools.ietf.org/html/rfc6229
 */
#ifndef RC4TEST_HPP
#define RC4TEST_HPP

#include "Test.hpp"
#include "../src/RC4.hpp"

namespace UnitTests
{
    class RC4Test : public Test
    {
    protected:
       RC4 *R;

       void setUp() override
       {
          R = new RC4(StringTest("Key").toBytes());
       }

       void tearDown() override
       {
          delete R;
       }
    };

    TEST(RC4Encode, RC4Test)
    {
       const StringTest clear_text("Plaintext");

       compare("BBF316E8D940AF0AD3", R->encode(clear_text.toBytes()).toHexString());
    }

    TEST(RC4Decode, RC4Test)
    {
       const StringTest clear_text("BBF316E8D940AF0AD3");

       compare("Plaintext", R->decode(clear_text.hexToBytes()).toString());
    }
}

#endif