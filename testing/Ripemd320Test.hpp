/*
 * Source : http://homes.esat.kuleuven.be/~bosselae/ripemd160/pdf/AB-9601/
 */
#ifndef RIPEMD320TEST_HPP
#define	RIPEMD320TEST_HPP

#include <gtest/gtest.h>
#include "../src/Ripemd.hpp"
#include "../src/Digest.hpp"

class Ripemd320Test : public ::testing::Test
{
protected:
   Ripemd320 *S;

   virtual void SetUp()
   {
      S = new Ripemd320();
   }

   virtual void TearDown()
   {
      delete S;
   }
};

TEST_F(Ripemd320Test, encodeNormalText)
{
   const std::string clear_text = "message digest";
   std::string hash = "3A8E28502ED45D422F68844F9DD316E7B98533FA3F2A91D29F84D425C88D6B4EFF727DF66A7C0197";

   EXPECT_EQ(hash, Vector::toHexString(S->encode(String::toBytes(clear_text))));
}

TEST_F(Ripemd320Test, encodeEmptyText)
{
   std::string hash = "22D65D5661536CDC75C1FDF5C6DE7B41B9F27325EBC61E8557177D705A0EC880151C3A32A00899B8";

   EXPECT_EQ(hash, Vector::toHexString(S->encode(std::vector<uint8_t>(0))));
}

#endif