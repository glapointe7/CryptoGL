
#ifndef LRCTEST_HPP
#define LRCTEST_HPP

#include <gtest/gtest.h>
#include "../src/LRC.hpp"

class LRCTest : public ::testing::Test
{
protected:
   LRC *S;
   virtual void SetUp()
   {
      S = new LRC();
   }
   
   virtual void TearDown()
   {
      delete S;
   }
};

TEST_F(LRCTest, encodeNormalText)
{
   const BytesVector message = {0x41, 0x30, 0x31, 0x54, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
      0x30, 0x30, 0x31, 0x30, 0x39, 0x39, 0x34, 0x35, 0x30, 0x30, 0x03};

   EXPECT_EQ(0x27, S->encode(message));
}

#endif