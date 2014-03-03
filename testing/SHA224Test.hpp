
#ifndef SHA224TEST_HPP
#define SHA224TEST_HPP

#include <gtest/gtest.h>
#include "../src/SHA2.hpp"

class SHA224Test : public ::testing::Test
{
protected:
   SHA224 *S;

   virtual void SetUp()
   {
      S = new SHA224();
   }

   virtual void TearDown()
   {
      delete S;
   }
};

TEST_F(SHA224Test, encodeNormalText)
{
   const StringTest clear_text = "The quick brown fox jumps over the lazy dog";
   const std::string hash = "730E109BD7A8A32B1CB9D9A09AA2325D2430587DDBC0C38BAD911525";

   EXPECT_EQ(hash, S->encode(clear_text.toBytes()).toHexString());
}

TEST_F(SHA224Test, encodeEmptyText)
{
   std::string hash = "D14A028C2A3A2BC9476102BB288234C415A2B01F828EA62AC5B3E42F";

   EXPECT_EQ(hash, S->encode(BytesVector()).toHexString());
}

TEST_F(SHA224Test, encodeHMACText)
{
   const StringTest clear_text = "Hi There";
   const BytesVector key = {0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 
      0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B};
   std::string hash = "896FB1128ABBDF196832107CD49DF33F47B4B1169912BA4F53684B22";

   EXPECT_EQ(hash, S->hmacEncode(key, clear_text.toBytes()).toHexString());
}

#endif