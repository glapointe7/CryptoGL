
#ifndef SHA256TEST_HPP
#define	SHA256TEST_HPP

#include <gtest/gtest.h>
#include "../src/SHA2.hpp"
#include "../src/Digest.hpp"

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

   EXPECT_EQ(hash, Digest::hexDigest(S->encode(Digest::getBytesFromString(clear_text))));
}

TEST_F(SHA256Test, encodeEmptyText)
{
   std::string hash = "E3B0C44298FC1C149AFBF4C8996FB92427AE41E4649B934CA495991B7852B855";

   EXPECT_EQ(hash, Digest::hexDigest(S->encode(std::vector<uint8_t>(0))));
}

TEST_F(SHA256Test, encodeHMACNormalText)
{
   const std::string clear_text = "Hi There";
   std::string hash = "B0344C61D8DB38535CA8AFCEAF0BF12B881DC200C9833DA726E9376C2E32CFF7";

   EXPECT_EQ(hash, Digest::hexDigest(S->hmacEncode(
      {0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B}, 
           Digest::getBytesFromString(clear_text))));
}

TEST_F(SHA256Test, encodeHMACEmptyText)
{
   std::string hash = "B613679A0814D9EC772F95D778C35FC5FF1697C493715653C6C712144292C5AD";

   EXPECT_EQ(hash, Digest::hexDigest(S->hmacEncode(std::vector<uint8_t>(0), std::vector<uint8_t>(0))));
}

#endif