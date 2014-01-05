/*
 * Test vectors from http://en.wikipedia.org/wiki/RC4#Test_vectors
 * http://tools.ietf.org/html/rfc6229
 */
#ifndef RC4TEST_HPP
#define RC4TEST_HPP

#include <gtest/gtest.h>
#include "../src/RC4.hpp"
#include "../src/Digest.hpp"

class RC4Test : public ::testing::Test
{
protected:
   RC4 *R;

   virtual void SetUp()
   {
      R = new RC4(String::toBytes("Key"));
   }

   virtual void TearDown()
   {
      delete R;
   }
};

TEST_F(RC4Test, encode)
{
   const BytesVector clear_text(String::toBytes("Plaintext"));
   
   EXPECT_EQ("BBF316E8D940AF0AD3", Vector::toHexString(R->encode(clear_text)));
}

TEST_F(RC4Test, decode)
{
   const BytesVector clear_text(String::toBytes("Plaintext"));

   EXPECT_EQ("Plaintext", Vector::toString(R->decode(String::hexToBytes("BBF316E8D940AF0AD3"))));
}

#endif