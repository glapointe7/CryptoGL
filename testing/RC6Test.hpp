
#ifndef RC6TEST_HPP
#define	RC6TEST_HPP

#include <gtest/gtest.h>
#include "../src/RC6.hpp"
#include "../src/Digest.hpp"

class RC6Test : public ::testing::Test
{
protected:
   RC6 *R, *C;

   virtual void SetUp()
   {
      R = new RC6(std::vector<uint8_t>(16, 0));
      C = new RC6({0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01, 0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78});
   }

   virtual void TearDown()
   {
      delete R;
      delete C;
   }
};

TEST_F(RC6Test, encodeZero)
{   
   EXPECT_EQ("8FC3A53656B1F778C129DF4E9848A41E", Digest::hexDigest(R->encode(std::vector<uint8_t>(16, 0))));
}

TEST_F(RC6Test, decodeZero)
{
   EXPECT_EQ("00000000000000000000000000000000", Digest::hexDigest(R->decode(Digest::getBytesFromHexDigest("8FC3A53656B1F778C129DF4E9848A41E"))));
}

TEST_F(RC6Test, encodeNormal)
{
   const std::vector<uint8_t> clear_text = {0x02, 0x13, 0x24, 0x35, 0x46, 0x57, 0x68, 0x79, 0x8A, 0x9B, 0xAC, 0xBD, 0xCE, 0xDF, 0xE0, 0xF1};
   EXPECT_EQ("524E192F4715C6231F51F6367EA43F18", Digest::hexDigest(C->encode(clear_text)));
}

TEST_F(RC6Test, decodeNormal)
{
   EXPECT_EQ("02132435465768798A9BACBDCEDFE0F1", Digest::hexDigest(C->decode(Digest::getBytesFromHexDigest("524E192F4715C6231F51F6367EA43F18"))));
}

#endif