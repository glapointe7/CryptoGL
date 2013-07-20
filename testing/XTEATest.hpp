
#ifndef XTEATEST_HPP
#define	XTEATEST_HPP

#include <gtest/gtest.h>
#include "../src/XTEA.hpp"
#include "../src/Digest.hpp"

class XTEATest : public ::testing::Test
{
protected:
   XTEA *R, *C;

   virtual void SetUp()
   {
      R = new XTEA(std::vector<uint8_t>(16, 0));
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
   EXPECT_EQ("DEE9D4D8F7131ED9", Digest::hexDigest(R->encode(std::vector<uint8_t>(8, 0))));
}

TEST_F(XTEATest, decodeZero)
{
   EXPECT_EQ("0000000000000000", Digest::hexDigest(R->decode(Digest::getBytesFromHexDigest("DEE9D4D8F7131ED9"))));
}

TEST_F(XTEATest, encodeNormal)
{
   const std::vector<uint8_t> clear_text = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
   EXPECT_EQ("27E795E076B2B537", Digest::hexDigest(C->encode(clear_text)));
}

TEST_F(XTEATest, decodeNormal)
{
   EXPECT_EQ("0123456789ABCDEF", Digest::hexDigest(C->decode(Digest::getBytesFromHexDigest("27E795E076B2B537"))));
}

#endif