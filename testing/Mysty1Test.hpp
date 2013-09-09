/*
 * Source : http://www.ipa.go.jp/security/rfc/RFC2994EN.html
 */
#ifndef MYSTY1TEST_HPP
#define MYSTY1TEST_HPP

#include <gtest/gtest.h>
#include "../src/Mysty1.hpp"
#include "../src/Digest.hpp"

class Mysty1Test : public ::testing::Test
{
protected:
   Mysty1 *ECB, *CBC;

   virtual void SetUp()
   {
      ECB = new Mysty1({0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF});
      CBC = new Mysty1({0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF},
                     OperationModes::CBC,{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08});
   }

   virtual void TearDown()
   {
      delete ECB;
      delete CBC;
   }
};

TEST_F(Mysty1Test, encodeECBMode)
{
   const std::vector<uint8_t> clear_text = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD,
      0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};

   EXPECT_EQ("8B1DA5F56AB3D07C04B68240B13BE95D", Digest::hexDigest(ECB->encode(clear_text)));
}

TEST_F(Mysty1Test, decodeECBMode)
{
   const std::vector<uint8_t> clear_text = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD,
      0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};

   EXPECT_EQ(Digest::hexDigest(clear_text), Digest::hexDigest(ECB->decode(Digest::getBytesFromHexDigest("8B1DA5F56AB3D07C04B68240B13BE95D"))));
}

TEST_F(Mysty1Test, encodeCBCMode)
{
   const std::vector<uint8_t> clear_text = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD,
      0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};

   EXPECT_EQ("461C1E879C18C27FB9ADF2D80C89031F", Digest::hexDigest(CBC->encode(clear_text)));
}

TEST_F(Mysty1Test, decodeCBCMode)
{
   const std::vector<uint8_t> clear_text = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD,
      0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};

   EXPECT_EQ(Digest::hexDigest(clear_text), Digest::hexDigest(CBC->decode(Digest::getBytesFromHexDigest("461C1E879C18C27FB9ADF2D80C89031F"))));
}

#endif