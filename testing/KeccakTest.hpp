
#ifndef KECCAKTEST_HPP
#define	KECCAKTEST_HPP

#include <gtest/gtest.h>
#include "../src/Keccak.hpp"
#include "../src/Digest.hpp"

class KeccakTest : public ::testing::Test
{
protected:
   Keccak224 *S;
   //Keccak256 *T;
   //Keccak384 *U;
   Keccak512 *V;

   virtual void SetUp()
   {
      S = new Keccak224();
      //T = new Keccak256();
      //U = new Keccak384();
      V = new Keccak512();
   }

   virtual void TearDown()
   {
      delete S;
      //delete T;
      //delete U;
      delete V;
   }
};

TEST_F(KeccakTest, encodeSimple224)
{
   const std::vector<uint8_t> clear_text = {0x84, 0xFB, 0x51, 0xB5, 0x17, 0xDF, 0x6C, 0x5A, 0xCC, 0xB5,
      0xD0, 0x22, 0xF8, 0xF2, 0x8D, 0xA0, 0x9B, 0x10, 0x23, 0x2D, 0x42, 0x32, 0x0F, 0xFC, 0x32, 0xDB, 0xEC,
      0xC3, 0x83, 0x5B, 0x29
   };
   std::string hash = "81AF3A7A5BD4C1F948D6AF4B96F93C3B0CF9C0E7A6DA6FCD71EEC7F6";

   EXPECT_EQ(hash, Digest::hexDigest(S->encode(clear_text)));
}

TEST_F(KeccakTest, encodeZero224)
{
   std::string hash = "F71837502BA8E10837BDD8D365ADB85591895602FC552B48B7390ABD";

   EXPECT_EQ(hash, Digest::hexDigest(S->encode(std::vector<uint8_t>(0))));
}

TEST_F(KeccakTest, encodeZero512)
{
   std::string hash = "0EAB42DE4C3CEB9235FC91ACFFE746B29C29A8C366B7C60E4E67C466F36A4304C"
           "00FA9CAF9D87976BA469BCBE06713B435F091EF2769FB160CDAB33D3670680E";

   EXPECT_EQ(hash, Digest::hexDigest(V->encode(std::vector<uint8_t>(0))));
}

#endif