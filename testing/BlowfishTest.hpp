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
   Blowfish *B, *CBC;

   virtual void SetUp()
   {
      B = new Blowfish({0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
      CBC = new Blowfish({0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xF0, 0xE1, 0xD2, 0xC3, 0xB4, 0xA5, 0x96, 0x87}, OperationModes::CBC);
   }

   virtual void TearDown()
   {
      delete B;
      delete CBC;
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

/*TEST_F(BlowfishTest, encodeWithCBCMode)
{
   const std::vector<uint8_t> clear_text = {0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x20, 0x4E, 0x6F, 
   0x77, 0x20, 0x69, 0x73, 0x20, 0x74, 0x68, 0x65, 0x20, 0x74, 0x69, 0x6D, 0x65, 0x20, 0x66, 0x6F, 0x72, 0x20, 0x00};
   
   CBC->setIV({0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10});
   EXPECT_EQ("6B77B4D63006DEE605B156E27403979358DEB9E7154616D959F1652BD5FF92CC", Digest::hexDigest(CBC->encode(clear_text)));
}

TEST_F(BlowfishTest, decodeWithCBCMode)
{   
   CBC->setIV({0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10});
   EXPECT_EQ("37363534333231204E6F77206973207468652074696D6520666F722000", 
           Digest::hexDigest(CBC->decode(Digest::getBytesFromHexDigest("6B77B4D63006DEE605B156E27403979358DEB9E7154616D959F1652BD5FF92CC"))));
}*/

#endif