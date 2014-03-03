/*
 * Source : https://www.cosic.esat.kuleuven.be/nessie/testvectors/bc/idea/Idea-128-64.verified.test-vectors
 */
#ifndef IDEATEST_HPP
#define IDEATEST_HPP

#include <gtest/gtest.h>
#include "../src/IDEA.hpp"

class IDEATest : public ::testing::Test
{
protected:
   IDEA *A, *B;

   virtual void SetUp()
   {
      A = new IDEA({0x9D, 0x40, 0x75, 0xC1, 0x03, 0xBC, 0x32, 0x2A, 0xFB, 0x03, 0xE7, 0xBE, 0x6A, 0xB3, 0x00, 0x06});
      B = new IDEA(BytesVector(16, 0));
   }

   virtual void TearDown()
   {
      delete A;
      delete B;
   }
};

TEST_F(IDEATest, encodeNotZero)
{
   const BytesVector clear_text = {0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08};
   
   EXPECT_EQ("F5DB1AC45E5EF9F9", A->encode(clear_text).toHexString());
}

TEST_F(IDEATest, decodeNotZero)
{
   const BytesVector clear_text(8, 8);
   const StringTest message("F5DB1AC45E5EF9F9");
   EXPECT_EQ(clear_text.toHexString(), A->decode(message.hexToBytes()).toHexString());
}

TEST_F(IDEATest, encodeZero)
{
   const BytesVector clear_text = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
   
   EXPECT_EQ("0001000100000000", B->encode(clear_text).toHexString());
}

TEST_F(IDEATest, decodeZero)
{
   const BytesVector clear_text(8, 0);
   const StringTest message("0001000100000000");
   EXPECT_EQ(clear_text.toHexString(), B->decode(message.hexToBytes()).toHexString());
}

#endif