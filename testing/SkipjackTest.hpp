
#ifndef SKIPJACKTEST_HPP
#define SKIPJACKTEST_HPP

#include <gtest/gtest.h>
#include "../src/Skipjack.hpp"
#include "../src/Digest.hpp"

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
   const std::vector<uint8_t> clear_text = {0x33, 0x22, 0x11, 0x00, 0xdd, 0xcc, 0xbb, 0xaa};
   
   EXPECT_EQ("2587CAE27A12D300", Digest::hexDigest(A->encode(clear_text)));
}

TEST_F(SkipjackTest, decodeFirst)
{
   const std::vector<uint8_t> clear_text = {0x33, 0x22, 0x11, 0x00, 0xdd, 0xcc, 0xbb, 0xaa};
   
   EXPECT_EQ(Digest::hexDigest(clear_text), Digest::hexDigest(A->decode(Digest::getBytesFromHexDigest("2587CAE27A12D300"))));
}

TEST_F(SkipjackTest, encodeSecond)
{
   const std::vector<uint8_t> clear_text = {0x1d, 0xdf, 0x39, 0xab, 0xf5, 0xcd, 0x71, 0x1e};
   
   EXPECT_EQ("C92D22324C6B31AE", Digest::hexDigest(B->encode(clear_text)));
}

TEST_F(SkipjackTest, decodeSecond)
{
   const std::vector<uint8_t> clear_text = {0x1d, 0xdf, 0x39, 0xab, 0xf5, 0xcd, 0x71, 0x1e};
   
   EXPECT_EQ(Digest::hexDigest(clear_text), Digest::hexDigest(B->decode(Digest::getBytesFromHexDigest("C92D22324C6B31AE"))));
}

#endif