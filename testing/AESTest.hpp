
#ifndef AESTEST_HPP
#define	AESTEST_HPP

#include <gtest/gtest.h>
#include "../src/AES.hpp"
#include "../src/Digest.hpp"

class AESTest : public ::testing::Test
{
protected:
   AES *A, *B, *C;

   virtual void SetUp()
   {
      A = new AES({0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f});
      B = new AES({0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
         0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17});
      C = new AES({0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
         0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F});
   }

   virtual void TearDown()
   {
      delete A;
      delete B;
      delete C;
   }
};

TEST_F(AESTest, encode128Bits)
{
   const std::vector<uint8_t> clear_text = {
      0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
   };

   EXPECT_EQ("69C4E0D86A7B0430D8CDB78070B4C55A", Digest::hexDigest(A->encode(clear_text)));
}

TEST_F(AESTest, encode192Bits)
{
   const std::vector<uint8_t> clear_text = {
      0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
   };

   EXPECT_EQ("DDA97CA4864CDFE06EAF70A0EC0D7191", Digest::hexDigest(B->encode(clear_text)));
}

TEST_F(AESTest, encode256Bits)
{
   const std::vector<uint8_t> clear_text = {
      0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
   };

   EXPECT_EQ("8EA2B7CA516745BFEAFC49904B496089", Digest::hexDigest(C->encode(clear_text)));
}

TEST_F(AESTest, decode128Bits)
{
   const std::vector<uint8_t> clear_text = {
      0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
   };

   EXPECT_EQ(Digest::hexDigest(clear_text), Digest::hexDigest(A->decode(Digest::getBytesFromHexDigest("69C4E0D86A7B0430D8CDB78070B4C55A"))));
}

TEST_F(AESTest, decode192Bits)
{
   const std::vector<uint8_t> clear_text = {
      0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
   };

   EXPECT_EQ(Digest::hexDigest(clear_text), Digest::hexDigest(B->decode(Digest::getBytesFromHexDigest("DDA97CA4864CDFE06EAF70A0EC0D7191"))));
}

TEST_F(AESTest, decode256Bits)
{
   const std::vector<uint8_t> clear_text = {
      0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
   };

   EXPECT_EQ(Digest::hexDigest(clear_text), Digest::hexDigest(C->decode(Digest::getBytesFromHexDigest("8EA2B7CA516745BFEAFC49904B496089"))));
}

#endif