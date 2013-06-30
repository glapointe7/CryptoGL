/*
 * Test vectors taken here : http://www.schneier.com/code/vectors.txt
 */
#ifndef BLOWFISHTEST_HPP
#define	BLOWFISHTEST_HPP

#include <gtest/gtest.h>
#include "../src/Blowfish.hpp"
#include "../src/Digest.hpp"

class BlowfishTest : public ::testing::Test
{
protected:
   Blowfish *B;

   virtual void SetUp()
   {
      B = new Blowfish({0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
   }

   virtual void TearDown()
   {
      delete B;
   }
};

TEST_F(BlowfishTest, encode)
{
   const std::vector<uint8_t> clear_text = {0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
   
   EXPECT_EQ("7D856F9A613063F2", Digest::hexDigest(B->encode(clear_text)));
}

TEST_F(BlowfishTest, decode)
{
   const std::vector<uint8_t> clear_text = {0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
   
   EXPECT_EQ(Digest::hexDigest(clear_text), Digest::hexDigest(B->decode(Digest::getBytesFromHexDigest("7D856F9A613063F2"))));
}

#endif