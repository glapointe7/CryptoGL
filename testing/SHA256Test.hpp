
#ifndef SHA256TEST_HPP
#define	SHA256TEST_HPP

#include <gtest/gtest.h>
#include "../src/SHA.hpp"

class SHA256Test : public ::testing::Test
{
protected:
   SHA256 *S;

   virtual void SetUp()
   {
      S = new SHA256();
   }

   virtual void TearDown()
   {
      delete S;
   }
};

TEST_F(SHA256Test, encodeNormalText)
{
   const std::string clear_text = "The quick brown fox jumps over the lazy dog";
   std::string hash = "D7A8FBB307D7809469CA9ABCB0082E4F8D5651E46D3CDB762D02D0BF37C9E592";

   EXPECT_EQ(hash, SHA256::hexDigest(S->encode(SHA256::getBytesFromString(clear_text))));
}

TEST_F(SHA256Test, encodeEmptyText)
{
   std::string hash = "E3B0C44298FC1C149AFBF4C8996FB92427AE41E4649B934CA495991B7852B855";

   EXPECT_EQ(hash, SHA256::hexDigest(S->encode(std::vector<uint8_t>(0))));
}

#endif