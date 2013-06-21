
#ifndef SHA224TEST_HPP
#define	SHA224TEST_HPP

#include <gtest/gtest.h>
#include "../src/SHA.hpp"

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
   const std::string clear_text = "The quick brown fox jumps over the lazy dog";
   std::string hash = "730E109BD7A8A32B1CB9D9A09AA2325D2430587DDBC0C38BAD911525";

   EXPECT_EQ(hash, SHA224::hexDigest(S->encode(SHA224::getBytesFromString(clear_text))));
}

TEST_F(SHA224Test, encodeEmptyText)
{
   std::string hash = "D14A028C2A3A2BC9476102BB288234C415A2B01F828EA62AC5B3E42F";

   EXPECT_EQ(hash, SHA224::hexDigest(S->encode(std::vector<uint8_t>(0))));
}

#endif