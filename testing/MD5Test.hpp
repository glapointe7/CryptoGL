/*
 * Tests from (http://tools.ietf.org/html/rfc1321)
 */
#ifndef MD5TEST_HPP
#define	MD5TEST_HPP

#include <gtest/gtest.h>
#include "../src/MD5.hpp"

class MD5Test : public ::testing::Test
{
protected:
   MD5 *S;

   virtual void SetUp()
   {
      S = new MD5();
   }

   virtual void TearDown()
   {
      delete S;
   }
};

TEST_F(MD5Test, encode)
{
   const std::string clear_text = "The quick brown fox jumps over the lazy dog";
   std::string hash = "9E107D9D372BB6826BD81D3542A419D6";

   EXPECT_EQ(hash, MD5::hexDigest(S->encode(MD5::getBytesFromString(clear_text))));
}

#endif