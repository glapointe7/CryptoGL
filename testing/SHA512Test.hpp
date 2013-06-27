
#ifndef SHA512TEST_HPP
#define	SHA512TEST_HPP

#include <gtest/gtest.h>
#include "../src/SHA.hpp"

class SHA512Test : public ::testing::Test
{
protected:
   SHA512 *S;

   virtual void SetUp()
   {
      S = new SHA512();
   }

   virtual void TearDown()
   {
      delete S;
   }
};

TEST_F(SHA512Test, encodeSimple)
{
   const std::string clear_text = "abc";
   std::string hash = "DDAF35A193617ABACC417349AE20413112E6FA4E89A97EA20A9EEEE64B55D39A2"
           "192992A274FC1A836BA3C23A3FEEBBD454D4423643CE80E2A9AC94FA54CA49F";

   EXPECT_EQ(hash, SHA512::hexDigest(S->encode(SHA512::getBytesFromString(clear_text))));
}

TEST_F(SHA512Test, encodeNormalText)
{
   const std::string clear_text = "The quick brown fox jumps over the lazy dog";
   std::string hash = "07E547D9586F6A73F73FBAC0435ED76951218FB7D0C8D788A309D785436BBB642"
           "E93A252A954F23912547D1E8A3B5ED6E1BFD7097821233FA0538F3DB854FEE6";

   EXPECT_EQ(hash, SHA512::hexDigest(S->encode(SHA512::getBytesFromString(clear_text))));
}

TEST_F(SHA512Test, encodeEmptyText)
{
   std::string hash = "CF83E1357EEFB8BDF1542850D66D8007D620E4050B5715DC83F4A921D36C"
           "E9CE47D0D13C5D85F2B0FF8318D2877EEC2F63B931BD47417A81A538327AF927DA3E";

   EXPECT_EQ(hash, SHA512::hexDigest(S->encode(std::vector<uint8_t>(0))));
}

#endif