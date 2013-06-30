/*
 * Source : http://homes.esat.kuleuven.be/~bosselae/ripemd160/pdf/AB-9601/
 */
#ifndef RIPEMD160TEST_HPP
#define	RIPEMD160TEST_HPP

#include <gtest/gtest.h>
#include "../src/Ripemd.hpp"
#include "../src/Digest.hpp"

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
   const std::string clear_text = "message digest";
   std::string hash = "5D0689EF49D2FAE572B881B123A85FFA21595F36";

   EXPECT_EQ(hash, Digest::hexDigest(S->encode(Digest::getBytesFromString(clear_text))));
}

TEST_F(Ripemd160Test, encodeEmptyText)
{
   std::string hash = "9C1185A5C5E9FC54612808977EE8F548B2258D31";

   EXPECT_EQ(hash, Digest::hexDigest(S->encode(std::vector<uint8_t>(0))));
}

#endif