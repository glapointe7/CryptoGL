
#ifndef SKIPJACKTEST_HPP
#define SKIPJACKTEST_HPP

#include <gtest/gtest.h>
#include "../src/Skipjack.hpp"

class SkipjackTest : public ::testing::Test
{
protected:
   Skipjack *A, *B;

   virtual void SetUp()
   {
      A = new Skipjack({0x00, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11});
      B = new Skipjack({0xf8, 0xda, 0x02, 0x64, 0x77, 0x22, 0xf7, 0x10, 0x3a, 0xdf});
   }

   virtual void TearDown()
   {
      delete A;
      delete B;
   }
};

TEST_F(SkipjackTest, encodeFirst)
{
   const BytesVector clear_text = {0x33, 0x22, 0x11, 0x00, 0xdd, 0xcc, 0xbb, 0xaa};
   
   EXPECT_EQ("2587CAE27A12D300", A->encode(clear_text).toHexString());
}

TEST_F(SkipjackTest, decodeFirst)
{
   const BytesVector clear_text = {0x33, 0x22, 0x11, 0x00, 0xdd, 0xcc, 0xbb, 0xaa};
   const StringTest message("2587CAE27A12D300");
   
   EXPECT_EQ(clear_text.toHexString(), A->decode(message.hexToBytes()).toHexString());
}

TEST_F(SkipjackTest, encodeSecond)
{
   const BytesVector clear_text = {0x1d, 0xdf, 0x39, 0xab, 0xf5, 0xcd, 0x71, 0x1e};
   
   EXPECT_EQ("C92D22324C6B31AE", B->encode(clear_text).toHexString());
}

TEST_F(SkipjackTest, decodeSecond)
{
   const BytesVector clear_text = {0x1d, 0xdf, 0x39, 0xab, 0xf5, 0xcd, 0x71, 0x1e};
   const StringTest message("C92D22324C6B31AE");
   
   EXPECT_EQ(clear_text.toHexString(), B->decode(message.hexToBytes()).toHexString());
}

#endif