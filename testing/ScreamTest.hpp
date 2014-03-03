
#ifndef SCREAMTEST_HPP
#define SCREAMTEST_HPP

#include <gtest/gtest.h>
#include "../src/Scream.hpp"

class ScreamTest : public ::testing::Test
{
protected:
   Scream_S *S;

   virtual void SetUp()
   {
      S = new Scream_S({0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10},
                     BytesVector(16, 0));
   }

   virtual void TearDown()
   {
      delete S;
   }
};

TEST_F(ScreamTest, generateS)
{
   const BytesVector keystream = S->generateKeystream();
   const BytesVector input = keystream.range(0, 16);
      
   EXPECT_EQ("748C59F20D769EA87A6DC18746E64AC0", input.toHexString());
}

#endif