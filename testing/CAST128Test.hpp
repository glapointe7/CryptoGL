/*
 * Source : http://tools.ietf.org/html/rfc2144
 */
#ifndef CAST128TEST_HPP
#define CAST128TEST_HPP

#include <gtest/gtest.h>
#include "../src/CAST128.hpp"
#include "../src/Digest.hpp"

class CAST128Test : public ::testing::Test
{
protected:
   CAST128 *C128, *C80, *C40;

   virtual void SetUp()
   {
      C128 = new CAST128({0x01, 0x23, 0x45, 0x67, 0x12, 0x34, 0x56, 0x78, 0x23, 0x45, 0x67, 0x89, 0x34, 0x56, 0x78, 0x9A});
      C80 = new CAST128({0x01, 0x23, 0x45, 0x67, 0x12, 0x34, 0x56, 0x78, 0x23, 0x45});
      C40 = new CAST128({0x01, 0x23, 0x45, 0x67, 0x12});
   }

   virtual void TearDown()
   {
      delete C128;
      delete C80;
      delete C40;
   }
};

TEST_F(CAST128Test, encode128)
{
   const std::vector<uint8_t> clear_text = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};

   EXPECT_EQ("238B4FE5847E44B2", Digest::hexDigest(C128->encode(clear_text)));
}

TEST_F(CAST128Test, decode128)
{
   EXPECT_EQ("0123456789ABCDEF", Digest::hexDigest(C128->decode(Digest::getBytesFromHexDigest("238B4FE5847E44B2"))));
}

TEST_F(CAST128Test, encode80)
{
   const std::vector<uint8_t> clear_text = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};

   EXPECT_EQ("EB6A711A2C02271B", Digest::hexDigest(C80->encode(clear_text)));
}

TEST_F(CAST128Test, decode80)
{
   EXPECT_EQ("0123456789ABCDEF", Digest::hexDigest(C80->decode(Digest::getBytesFromHexDigest("EB6A711A2C02271B"))));
}

TEST_F(CAST128Test, encode40)
{
   const std::vector<uint8_t> clear_text = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};

   EXPECT_EQ("7AC816D16E9B302E", Digest::hexDigest(C40->encode(clear_text)));
}

TEST_F(CAST128Test, decode40)
{
   EXPECT_EQ("0123456789ABCDEF", Digest::hexDigest(C40->decode(Digest::getBytesFromHexDigest("7AC816D16E9B302E"))));
}

#endif