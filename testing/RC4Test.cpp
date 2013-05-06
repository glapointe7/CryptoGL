/*
 * Test vectors from http://en.wikipedia.org/wiki/RC4#Test_vectors
 * http://tools.ietf.org/html/rfc6229
 */

#ifndef RC4TEST_HPP
#define	RC4TEST_HPP

#include <gtest/gtest.h>
#include "../src/RC4.hpp"

class RC4Test : public ::testing::Test
{
protected:
   RC4 *R;

   virtual void SetUp()
   {
      R = new RC4();
   }

   virtual void TearDown()
   {
      delete R;
   }
};

TEST_F(RC4Test, encode)
{
   const std::vector<uint8_t> clear_text(RC4::getBytesFromString("Plaintext"));
   const std::vector<uint8_t> key(RC4::getBytesFromString("Key"));
   
   R->setKey(key);
   EXPECT_EQ("BBF316E8D940AF0AD3", RC4::hexDigest(R->encode(clear_text)));
}

TEST_F(RC4Test, decode)
{
   const std::vector<uint8_t> clear_text(RC4::getBytesFromString("Plaintext"));
   const std::vector<uint8_t> key(RC4::getBytesFromString("Key"));
   
   R->setKey(key);
   EXPECT_EQ("Plaintext", RC4::getStringFromBytes(R->decode(RC4::getBytesFromHexDigest("BBF316E8D940AF0AD3"))));
}

#endif