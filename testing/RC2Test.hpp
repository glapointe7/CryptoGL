
#ifndef RC2TEST_HPP
#define RC2TEST_HPP

#include <gtest/gtest.h>
#include "../src/RC2.hpp"

class RC2Test : public ::testing::Test
{
protected:
   RC2 *R, *C;

   virtual void SetUp()
   {
      R = new RC2({0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
      C = new RC2({0x88, 0xbc, 0xa9, 0x0e, 0x90, 0x87, 0x5a, 0x7f, 0x0f, 0x79, 0xc3, 0x84, 0x62, 0x7b, 0xaf, 0xb2});
   }

   virtual void TearDown()
   {
      delete R;
      delete C;
   }
};

TEST_F(RC2Test, encodeWith8BytesKey)
{   
   const BytesVector clear_text = {0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
   EXPECT_EQ("30649EDF9BE7D2C2", R->encode(clear_text).toHexString());
}

TEST_F(RC2Test, decodeWith8BytesKey)
{
   EXPECT_EQ("1000000000000001", R->decode(StringTest("30649EDF9BE7D2C2").hexToBytes()).toHexString());
}

TEST_F(RC2Test, encodeWith16BytesKey)
{   
   EXPECT_EQ("2269552AB0F85CA6", C->encode(BytesVector(8, 0)).toHexString());
}

TEST_F(RC2Test, decodeWith16BytesKey)
{
   EXPECT_EQ("0000000000000000", C->decode(StringTest("2269552AB0F85CA6").hexToBytes()).toHexString());
}

#endif