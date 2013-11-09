
#ifndef ADLER32TEST_HPP
#define ADLER32TEST_HPP

#include <gtest/gtest.h>
#include "../src/Adler32.hpp"
#include "../src/Digest.hpp"

class Adler32Test : public ::testing::Test
{
protected:
   Adler32 *S;
   virtual void SetUp()
   {
      S = new Adler32();
   }
   
   virtual void TearDown()
   {
      delete S;
   }
};
TEST_F(Adler32Test, encodeEmpty)
{
   EXPECT_EQ(0x00000001, S->encode({}));
}

TEST_F(Adler32Test, encodeNormal)
{
   EXPECT_EQ(0x29750586, S->encode(Digest::getBytesFromString("message digest")));
}

#endif