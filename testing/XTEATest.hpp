
#ifndef XTEATEST_HPP
#define XTEATEST_HPP

#include <gtest/gtest.h>
#include "../src/XTEA.hpp"

class XTEATest : public ::testing::Test
{
protected:
   XTEA *R, *C;

   virtual void SetUp()
   {
      R = new XTEA(BytesVector(16, 0));
      C = new XTEA({0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef});
   }

   virtual void TearDown()
   {
      delete R;
      delete C;
   }
};

TEST_F(XTEATest, encodeZero)
{   
   EXPECT_EQ("DEE9D4D8F7131ED9", R->encode(BytesVector(8, 0)).toHexString());
}

TEST_F(XTEATest, decodeZero)
{
   EXPECT_EQ("0000000000000000", R->decode(StringTest("DEE9D4D8F7131ED9").hexToBytes()).toHexString());
}

TEST_F(XTEATest, encodeNormal)
{
   const BytesVector clear_text = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
   EXPECT_EQ("27E795E076B2B537", C->encode(clear_text).toHexString());
}

TEST_F(XTEATest, decodeNormal)
{
   EXPECT_EQ("0123456789ABCDEF", C->decode(StringTest("27E795E076B2B537").hexToBytes()).toHexString());
}

#endif