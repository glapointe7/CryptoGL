
#ifndef MD4TEST_HPP
#define	MD4TEST_HPP

#include <gtest/gtest.h>
#include "../src/MessageDigest.hpp"
#include "../src/Digest.hpp"

class MD4Test : public ::testing::Test
{
protected:
   MD4 *S;

   virtual void SetUp()
   {
      S = new MD4();
   }

   virtual void TearDown()
   {
      delete S;
   }
};

TEST_F(MD4Test, encodeNormalText)
{
   const std::string clear_text = "message digest";
   std::string hash = "D9130A8164549FE818874806E1C7014B";

   EXPECT_EQ(hash, Digest::hexDigest(S->encode(Digest::getBytesFromString(clear_text))));
}

TEST_F(MD4Test, encodeEmptyText)
{
   std::string hash = "31D6CFE0D16AE931B73C59D7E0C089C0";

   EXPECT_EQ(hash, Digest::hexDigest(S->encode(std::vector<uint8_t>(0))));
}

#endif