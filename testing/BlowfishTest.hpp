
#ifndef BLOWFISHTEST_HPP
#define	BLOWFISHTEST_HPP

#include <gtest/gtest.h>
#include "../src/Blowfish.hpp"

class BlowfishTest : public ::testing::Test
{
protected:
   Blowfish *B;

   virtual void SetUp()
   {
      B = new Blowfish();
   }

   virtual void TearDown()
   {
      delete B;
   }
};

TEST_F(BlowfishTest, encode)
{
   const std::vector<uint8_t> clear_text(8, 0);
   const std::vector<uint8_t> key(8, 0);
   
   B->setKey(key);
   EXPECT_EQ("4EF997456198DD78", Blowfish::hexDigest(B->encode(clear_text)));
}

TEST_F(BlowfishTest, decode)
{
   const std::vector<uint8_t> clear_text(8, 0);
   const std::vector<uint8_t> key(8, 0);
   
   B->setKey(key);
   EXPECT_EQ(Blowfish::hexDigest(clear_text), 
           Blowfish::hexDigest(B->decode(Blowfish::getBytesFromHexDigest("4EF997456198DD78"))));
}

#endif