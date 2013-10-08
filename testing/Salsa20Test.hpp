/*
 * Source : http://www.ecrypt.eu.org/stream/svn/viewcvs.cgi/ecrypt/trunk/submissions/salsa20/full/verified.test-vectors?rev=210&view=markup
 */
#ifndef SALSA20TEST_HPP
#define SALSA20TEST_HPP

#include <gtest/gtest.h>
#include "../src/Salsa20.hpp"
#include "../src/Digest.hpp"

class Salsa20Test : public ::testing::Test
{
protected:
   Salsa20 *R, *S128;

   virtual void SetUp()
   {
      R = new Salsa20(BytesVector(32, 0), BytesVector(8, 0));
      S128 = new Salsa20({0x00, 0x53, 0xA6, 0xF9, 0x4C, 0x9F, 0xF2, 0x45, 0x98, 0xEB, 0x3E, 0x91, 0xE4, 0x37, 0x8A, 0xDD},
         {0x0D, 0x74, 0xDB, 0x42, 0xA9, 0x10, 0x77, 0xDE});
   }

   virtual void TearDown()
   {
      delete R;
      delete S128;
   }
};

TEST_F(Salsa20Test, generate256Zero)
{
   const std::string expected = "9A97F65B9B4C721B960A672145FCA8D4E32E67F9111EA979CE9C4826806AEEE6"
           "3DE9C0DA2BD7F91EBCB2639BF989C6251B29BF38D39A9BDCE7C55F4B2AC12A39";
   
   EXPECT_EQ(expected, Digest::hexDigest(R->generateKeystream()));
}

TEST_F(Salsa20Test, generate128)
{
   const std::string expected = "05E1E7BEB697D999656BF37C1B978806735D0B903A6007BD329927EFBE1B0E2A"
             "8137C1AE291493AA83A821755BEE0B06CD14855A67E46703EBF8F3114B584CBA";
   
   EXPECT_EQ(expected, Digest::hexDigest(S128->generateKeystream()));
}

#endif