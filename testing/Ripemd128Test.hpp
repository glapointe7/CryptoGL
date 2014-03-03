/*
 * Source : http://homes.esat.kuleuven.be/~bosselae/ripemd160/pdf/AB-9601/
 */
#ifndef RIPEMD128TEST_HPP
#define RIPEMD128TEST_HPP

#include <gtest/gtest.h>
#include "../src/Ripemd.hpp"

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
   const StringTest clear_text = "message digest";
   std::string hash = "9E327B3D6E523062AFC1132D7DF9D1B8";

   EXPECT_EQ(hash, S->encode(clear_text.toBytes()).toHexString());
}

TEST_F(Ripemd128Test, encodeEmptyText)
{
   std::string hash = "CDF26213A150DC3ECB610F18F6B38B46";

   EXPECT_EQ(hash, S->encode(BytesVector()).toHexString());
}

TEST_F(Ripemd128Test, encodeHMACNormalText)
{
   const StringTest clear_text = "message digest";
   std::string hash = "E8503A8AEC2289D82AA0D8D445A06BDD";
   const BytesVector key = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
      0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

   EXPECT_EQ(hash, S->hmacEncode(key, clear_text.toBytes()).toHexString());
}

TEST_F(Ripemd128Test, encodeHMACEmptyText)
{
   const BytesVector key = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
      0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
   std::string hash = "AD9DB2C1E22AF9AB5CA9DBE5A86F67DC";

   EXPECT_EQ(hash, S->hmacEncode(key, BytesVector()).toHexString());
}

#endif