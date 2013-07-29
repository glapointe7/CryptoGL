
#ifndef WHIRLPOOLTEST_HPP
#define	WHIRLPOOLTEST_HPP

#include <gtest/gtest.h>
#include "../src/Whirlpool.hpp"
#include "../src/Digest.hpp"

class WhirlpoolTest : public ::testing::Test
{
protected:
   Whirlpool *S;

   virtual void SetUp()
   {
      S = new Whirlpool();
   }

   virtual void TearDown()
   {
      delete S;
   }
};

TEST_F(WhirlpoolTest, encodeZero)
{
   std::string hash = "19FA61D75522A4669B44E39C1D2E1726C530232130D407F89AFEE0964997F7A73E83B"
           "E698B288FEBCF88E3E03C4F0757EA8964E59B63D93708B138CC42A66EB3";
   
   EXPECT_EQ(hash, Digest::hexDigest(S->encode(std::vector<uint8_t>(0))));
}

TEST_F(WhirlpoolTest, encodeText)
{
   const std::string clear_text = "message digest";
   std::string hash = "378C84A4126E2DC6E56DCC7458377AAC838D00032230F53CE1F5700C0FFB4D3B842155"
           "7659EF55C106B4B52AC5A4AAA692ED920052838F3362E86DBD37A8903E";
   
   EXPECT_EQ(hash, Digest::hexDigest(S->encode(Digest::getBytesFromString(clear_text))));
}

#endif