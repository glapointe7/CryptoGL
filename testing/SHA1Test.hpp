#ifndef SHA1TEST_HPP
#define SHA1TEST_HPP

#include <gtest/gtest.h>
#include "../src/SHA1.hpp"

class SHA1Test : public ::testing::Test
{
protected:
   SHA1 *S;

   virtual void SetUp()
   {
      S = new SHA1();
   }

   virtual void TearDown()
   {
      delete S;
   }
};

TEST_F(SHA1Test, encode)
{  
   EXPECT_EQ("DA39A3EE5E6B4B0D3255BFEF95601890AFD80709", S->encode(BytesVector()).toHexString());
}

TEST_F(SHA1Test, encodeHMACEmptyText)
{
   EXPECT_EQ("FBDB1D1B18AA6C08324B7D64B71FB76370690E1D", S->hmacEncode(BytesVector(), BytesVector()).toHexString());
}

TEST_F(SHA1Test, encodeHMACText)
{
   const StringTest clear_text = "The quick brown fox jumps over the lazy dog";
   const StringTest hash = "DE7C9B85B8B78AA6BC8A7A36F70A90701C9DB4D9";
   const StringTest hmac_key = "key";
   
   EXPECT_EQ(hash, S->hmacEncode(hmac_key.toBytes(), 
           clear_text.toBytes()).toHexString());
}

#endif