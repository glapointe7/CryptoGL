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

   EXPECT_EQ(hash, Digest::hexDigest(S->encode(Digest::getBytesFromString(clear_text))));
}

TEST_F(Ripemd128Test, encodeEmptyText)
{
   std::string hash = "CDF26213A150DC3ECB610F18F6B38B46";

   EXPECT_EQ(hash, Digest::hexDigest(S->encode(std::vector<uint8_t>(0))));
}

#endif