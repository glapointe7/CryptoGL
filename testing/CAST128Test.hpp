/*
 * Source : http://tools.ietf.org/html/rfc2144
 */
#ifndef CAST128TEST_HPP
#define CAST128TEST_HPP

#include <gtest/gtest.h>
#include "../src/CAST128.hpp"

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
   const BytesVector clear_text = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};

   EXPECT_EQ("238B4FE5847E44B2", C128->encode(clear_text).toHexString());
}

TEST_F(CAST128Test, decode128)
{
   StringTest code("238B4FE5847E44B2");
   EXPECT_EQ("0123456789ABCDEF", C128->decode(code.hexToBytes()).toHexString());
}

TEST_F(CAST128Test, encode80)
{
   const BytesVector clear_text = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};

   EXPECT_EQ("EB6A711A2C02271B", C80->encode(clear_text).toHexString());
}

TEST_F(CAST128Test, decode80)
{
   StringTest code("EB6A711A2C02271B");
   EXPECT_EQ("0123456789ABCDEF", C80->decode(code.hexToBytes()).toHexString());
}

TEST_F(CAST128Test, encode40)
{
   const BytesVector clear_text = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};

   EXPECT_EQ("7AC816D16E9B302E", C40->encode(clear_text).toHexString());
}

TEST_F(CAST128Test, decode40)
{
   StringTest code("7AC816D16E9B302E");
   EXPECT_EQ("0123456789ABCDEF", C40->decode(code.hexToBytes()).toHexString());
}

#endif