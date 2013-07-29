#ifndef SHA1TEST_HPP
#define SHA1TEST_HPP

#include <gtest/gtest.h>
#include "../src/SHA1.hpp"
#include "../src/Digest.hpp"

class SHA1Test : public ::testing::Test
{
protected:
   SHA1 *S;

   virtual void SetUp()
   {
      S = new SHA1();
   }

   virtual void TearDown()
   {
      delete S;
   }
};

TEST_F(SHA1Test, encode)
{
   const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
   std::string hash = "F5E6523299AC71B900615F641A4A74DB4C19A0AB";
   
   EXPECT_EQ(hash, Digest::hexDigest(S->encode(Digest::getBytesFromString(clear_text))));
}

TEST_F(SHA1Test, encodeHMACEmptyText)
{
   std::string hash = "FBDB1D1B18AA6C08324B7D64B71FB76370690E1D";
   
   EXPECT_EQ(hash, Digest::hexDigest(S->hmacEncode(std::vector<uint8_t>(0), std::vector<uint8_t>(0))));
}

TEST_F(SHA1Test, encodeHMACText)
{
   const std::string clear_text = "The quick brown fox jumps over the lazy dog";
   std::string hash = "DE7C9B85B8B78AA6BC8A7A36F70A90701C9DB4D9";
   
   EXPECT_EQ(hash, Digest::hexDigest(S->hmacEncode(Digest::getBytesFromString("key"), Digest::getBytesFromString(clear_text))));
}

#endif