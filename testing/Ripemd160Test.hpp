/*
 * Source : http://homes.esat.kuleuven.be/~bosselae/ripemd160/pdf/AB-9601/
 */
#ifndef RIPEMD160TEST_HPP
#define RIPEMD160TEST_HPP

#include <gtest/gtest.h>
#include "../src/Ripemd.hpp"

class Ripemd160Test : public ::testing::Test
{
protected:
   Ripemd160 *S;

   virtual void SetUp()
   {
      S = new Ripemd160();
   }

   virtual void TearDown()
   {
      delete S;
   }
};

TEST_F(Ripemd160Test, encodeNormalText)
{
   const StringTest clear_text = "message digest";
   std::string hash = "5D0689EF49D2FAE572B881B123A85FFA21595F36";

   EXPECT_EQ(hash, Vector::toHexString(S->encode(clear_text.toBytes())));
}

TEST_F(Ripemd160Test, encodeEmptyText)
{
   std::string hash = "9C1185A5C5E9FC54612808977EE8F548B2258D31";

   EXPECT_EQ(hash, Vector::toHexString(S->encode(BytesVector(0))));
}

TEST_F(Ripemd160Test, encodeHMACNormalText)
{
   const StringTest clear_text = "message digest";
   std::string hash = "F83662CC8D339C227E600FCD636C57D2571B1C34";
   const BytesVector key = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
      0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x01, 0x23, 0x45, 0x67};

   EXPECT_EQ(hash, Vector::toHexString(S->hmacEncode(key, clear_text.toBytes())));
}

TEST_F(Ripemd160Test, encodeHMACEmptyText)
{
   const BytesVector key = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
      0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x01, 0x23, 0x45, 0x67};
   std::string hash = "CF387677BFDA8483E63B57E06C3B5ECD8B7FC055";

   EXPECT_EQ(hash, Vector::toHexString(S->hmacEncode(key, BytesVector(0))));
}

#endif