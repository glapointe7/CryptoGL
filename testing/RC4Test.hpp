/*
 * Test vectors from http://en.wikipedia.org/wiki/RC4#Test_vectors
 * http://tools.ietf.org/html/rfc6229
 */
#ifndef RC4TEST_HPP
#define RC4TEST_HPP

#include <gtest/gtest.h>
#include "../src/RC4.hpp"

class RC4Test : public ::testing::Test
{
protected:
   RC4 *R;

   virtual void SetUp()
   {
      R = new RC4(StringTest("Key").toBytes());
   }

   virtual void TearDown()
   {
      delete R;
   }
};

TEST_F(RC4Test, encode)
{
   const StringTest clear_text("Plaintext");
   
   EXPECT_EQ("BBF316E8D940AF0AD3", R->encode(clear_text.toBytes()).toHexString());
}

TEST_F(RC4Test, decode)
{
   const StringTest clear_text("BBF316E8D940AF0AD3");

   EXPECT_EQ("Plaintext", R->decode(clear_text.hexToBytes()).toString());
}

#endif