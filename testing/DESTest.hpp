#ifndef DESTEST_HPP
#define DESTEST_HPP

#include <gtest/gtest.h>
#include "../src/DES.hpp"

class DESTest : public ::testing::Test
{
protected:
   DES *D;

   virtual void SetUp()
   {
      D = new DES({0x13, 0x34, 0x57, 0x79, 0x9B, 0xBC, 0xDF, 0xF1});
   }

   virtual void TearDown()
   {
      delete D;
   }
};

TEST_F(DESTest, encode)
{
   const std::vector<uint8_t> clear_text = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
   
   EXPECT_EQ("85E813540F0AB405", DES::hexDigest(D->encode(clear_text)));
}

TEST_F(DESTest, decode)
{
   const std::vector<uint8_t> clear_text = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
   
   EXPECT_EQ(DES::hexDigest(clear_text), DES::hexDigest(D->decode(DES::getBytesFromHexDigest("85E813540F0AB405"))));
}

#endif