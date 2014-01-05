/*
 * Source : http://homes.esat.kuleuven.be/~bosselae/ripemd160/pdf/AB-9601/
 */
#ifndef RIPEMD128TEST_HPP
#define	RIPEMD128TEST_HPP

#include <gtest/gtest.h>
#include "../src/Ripemd.hpp"
#include "../src/Digest.hpp"

class Ripemd128Test : public ::testing::Test
{
protected:
   Ripemd128 *S;

   virtual void SetUp()
   {
      S = new Ripemd128();
   }

   virtual void TearDown()
   {
      delete S;
   }
};

TEST_F(Ripemd128Test, encodeNormalText)
{
   const std::string clear_text = "message digest";
   std::string hash = "9E327B3D6E523062AFC1132D7DF9D1B8";

   EXPECT_EQ(hash, Vector::toHexString(S->encode(String::toBytes(clear_text))));
}

TEST_F(Ripemd128Test, encodeEmptyText)
{
   std::string hash = "CDF26213A150DC3ECB610F18F6B38B46";

   EXPECT_EQ(hash, Vector::toHexString(S->encode(std::vector<uint8_t>(0))));
}

TEST_F(Ripemd128Test, encodeHMACNormalText)
{
   const std::string clear_text = "message digest";
   std::string hash = "E8503A8AEC2289D82AA0D8D445A06BDD";
   const std::vector<uint8_t> key = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
      0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

   EXPECT_EQ(hash, Vector::toHexString(S->hmacEncode(key, String::toBytes(clear_text))));
}

TEST_F(Ripemd128Test, encodeHMACEmptyText)
{
   const std::vector<uint8_t> key = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
      0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
   std::string hash = "AD9DB2C1E22AF9AB5CA9DBE5A86F67DC";

   EXPECT_EQ(hash, Vector::toHexString(S->hmacEncode(key, std::vector<uint8_t>(0))));
}

#endif