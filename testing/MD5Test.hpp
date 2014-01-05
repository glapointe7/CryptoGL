/*
 * Tests from (http://tools.ietf.org/html/rfc1321)
 */
#ifndef MD5TEST_HPP
#define	MD5TEST_HPP

#include <gtest/gtest.h>
#include "../src/MessageDigest.hpp"
#include "../src/Digest.hpp"

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

TEST_F(MD5Test, encodeNormalText)
{
   const std::string clear_text = "The quick brown fox jumps over the lazy dog";
   std::string hash = "9E107D9D372BB6826BD81D3542A419D6";

   EXPECT_EQ(hash, Vector::toHexString(S->encode(String::toBytes(clear_text))));
}

TEST_F(MD5Test, encodeEmptyText)
{
   std::string hash = "D41D8CD98F00B204E9800998ECF8427E";

   EXPECT_EQ(hash, Vector::toHexString(S->encode(std::vector<uint8_t>(0))));
}

TEST_F(MD5Test, encodeHMACNormalText)
{
   const std::string clear_text = "The quick brown fox jumps over the lazy dog";
   std::string hash = "80070713463E7749B90C2DC24911E275";

   EXPECT_EQ(hash, Vector::toHexString(S->hmacEncode(String::toBytes("key"), String::toBytes(clear_text))));
}

TEST_F(MD5Test, encodeHMACEmptyText)
{
   std::string hash = "74E6F7298A9C2D168935F58C001BAD88";

   EXPECT_EQ(hash, Vector::toHexString(S->hmacEncode(std::vector<uint8_t>(0), std::vector<uint8_t>(0))));
}

#endif