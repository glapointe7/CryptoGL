/*
 * Source : http://tools.ietf.org/html/rfc3713
 */
#ifndef CAMELLIATEST_HPP
#define CAMELLIATEST_HPP

#include <gtest/gtest.h>
#include "../src/Camellia.hpp"
#include "../src/Digest.hpp"

class CamelliaTest : public ::testing::Test
{
protected:
   Camellia *C128, *C192, *C256;

   virtual void SetUp()
   {
      C128 = new Camellia({0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10});
      C192 = new Camellia({0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10,
         0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77});
      C256 = new Camellia({0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10,
         0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF});
   }

   virtual void TearDown()
   {
      delete C128;
      delete C192;
      delete C256;
   }
};

TEST_F(CamelliaTest, encode128)
{
   const std::vector<uint8_t> clear_text = {
      0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10
   };

   EXPECT_EQ("67673138549669730857065648EABE43", Digest::hexDigest(C128->encode(clear_text)));
}

TEST_F(CamelliaTest, decode128)
{
   EXPECT_EQ("0123456789ABCDEFFEDCBA9876543210", 
           Digest::hexDigest(C128->decode(Digest::getBytesFromHexDigest("67673138549669730857065648EABE43"))));
}

TEST_F(CamelliaTest, encode192)
{
   const std::vector<uint8_t> clear_text = {
      0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10
   };

   EXPECT_EQ("B4993401B3E996F84EE5CEE7D79B09B9", Digest::hexDigest(C192->encode(clear_text)));
}

TEST_F(CamelliaTest, decode192)
{
   EXPECT_EQ("0123456789ABCDEFFEDCBA9876543210", 
           Digest::hexDigest(C192->decode(Digest::getBytesFromHexDigest("B4993401B3E996F84EE5CEE7D79B09B9"))));
}

TEST_F(CamelliaTest, encode256)
{
   const std::vector<uint8_t> clear_text = {
      0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10
   };

   EXPECT_EQ("9ACC237DFF16D76C20EF7C919E3A7509", Digest::hexDigest(C256->encode(clear_text)));
}

TEST_F(CamelliaTest, decode256)
{
   EXPECT_EQ("0123456789ABCDEFFEDCBA9876543210", 
           Digest::hexDigest(C256->decode(Digest::getBytesFromHexDigest("9ACC237DFF16D76C20EF7C919E3A7509"))));
}

#endif