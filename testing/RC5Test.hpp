
#ifndef RC5TEST_HPP
#define RC5TEST_HPP

#include <gtest/gtest.h>
#include "../src/RC5.hpp"

class RC5Test : public ::testing::Test
{
protected:
   RC5 *R, *C;

   virtual void SetUp()
   {
      R = new RC5(BytesVector(16, 0));
      C = new RC5({0x91, 0x5F, 0x46, 0x19, 0xBE, 0x41, 0xB2, 0x51, 0x63, 0x55, 0xA5, 0x01, 0x10, 0xA9, 0xCE, 0x91});
   }

   virtual void TearDown()
   {
      delete R;
      delete C;
   }
};

TEST_F(RC5Test, encodeZero)
{   
   EXPECT_EQ("21A5DBEE154B8F6D", Vector::toHexString(R->encode(BytesVector(8, 0))));
}

TEST_F(RC5Test, decodeZero)
{
   EXPECT_EQ("0000000000000000", Vector::toHexString(R->decode(StringTest("21A5DBEE154B8F6D").hexToBytes())));
}

TEST_F(RC5Test, encodeNormal)
{
   const BytesVector clear_text = {0x21, 0xA5, 0xDB, 0xEE, 0x15, 0x4B, 0x8F, 0x6D};
   EXPECT_EQ("F7C013AC5B2B8952", Vector::toHexString(C->encode(clear_text)));
}

TEST_F(RC5Test, decodeNormal)
{
   EXPECT_EQ("21A5DBEE154B8F6D", Vector::toHexString(C->decode(StringTest("F7C013AC5B2B8952").hexToBytes())));
}

#endif