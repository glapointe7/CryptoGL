/*
 * Source : http://homes.esat.kuleuven.be/~bosselae/ripemd160/pdf/AB-9601/
 */
#ifndef RIPEMD256TEST_HPP
#define	RIPEMD256TEST_HPP

#include <gtest/gtest.h>
#include "../src/Ripemd.hpp"

class Ripemd256Test : public ::testing::Test
{
protected:
   Ripemd256 *S;

   virtual void SetUp()
   {
      S = new Ripemd256();
   }

   virtual void TearDown()
   {
      delete S;
   }
};

TEST_F(Ripemd256Test, encodeNormalText)
{
   const std::string clear_text = "message digest";
   std::string hash = "87E971759A1CE47A514D5C914C392C9018C7C46BC14465554AFCDF54A5070C0E";

   EXPECT_EQ(hash, Ripemd256::hexDigest(S->encode(Ripemd256::getBytesFromString(clear_text))));
}

TEST_F(Ripemd256Test, encodeEmptyText)
{
   std::string hash = "02BA4C4E5F8ECD1877FC52D64D30E37A2D9774FB1E5D026380AE0168E3C5522D";

   EXPECT_EQ(hash, Ripemd256::hexDigest(S->encode(std::vector<uint8_t>(0))));
}

#endif