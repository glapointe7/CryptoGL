
#ifndef SHA512_224TEST_HPP
#define SHA512_224TEST_HPP

#include <gtest/gtest.h>
#include "../src/SHA2.hpp"

class SHA512_224Test : public ::testing::Test
{
protected:
   SHA512_224 *S;

   virtual void SetUp()
   {
      S = new SHA512_224();
   }

   virtual void TearDown()
   {
      delete S;
   }
};

TEST_F(SHA512_224Test, encodeNormalText)
{
   const StringTest clear_text = "The quick brown fox jumps over the lazy dog";
   std::string hash = "944CD2847FB54558D4775DB0485A50003111C8E5DAA63FE722C6AA37";

   EXPECT_EQ(hash, S->encode(clear_text.toBytes()).toHexString());
}

TEST_F(SHA512_224Test, encodeEmptyText)
{
   std::string hash = "6ED0DD02806FA89E25DE060C19D3AC86CABB87D6A0DDD05C333B84F4";

   EXPECT_EQ(hash, S->encode(BytesVector()).toHexString());
}

#endif