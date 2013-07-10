
#ifndef TIGERTEST_HPP
#define	TIGERTEST_HPP

#include <gtest/gtest.h>
#include "../src/Tiger.hpp"
#include "../src/Digest.hpp"

class TigerTest : public ::testing::Test
{
protected:
   Tiger128 *T1;
   Tiger160 *T2;
   Tiger192 *T3;

   virtual void SetUp()
   {
      T1 = new Tiger128();
      T2 = new Tiger160();
      T3 = new Tiger192();
   }

   virtual void TearDown()
   {
      delete T1;
      delete T2;
      delete T3;
   }
};

TEST_F(TigerTest, encodeNormalTiger128)
{
   const std::string clear_text = "The quick brown fox jumps over the lazy dog";
   std::string hash = "6D12A41E72E644F017B6F0E2F7B44C62";

   EXPECT_EQ(hash, Digest::hexDigest(T1->encode(Digest::getBytesFromString(clear_text))));
}

TEST_F(TigerTest, encodeEmptyTiger128)
{
   std::string hash = "3293AC630C13F0245F92BBB1766E1616";

   EXPECT_EQ(hash, Digest::hexDigest(T1->encode(std::vector<uint8_t>(0))));
}

TEST_F(TigerTest, encodeNormalTiger160)
{
   const std::string clear_text = "The quick brown fox jumps over the lazy dog";
   std::string hash = "6D12A41E72E644F017B6F0E2F7B44C6285F06DD5";

   EXPECT_EQ(hash, Digest::hexDigest(T2->encode(Digest::getBytesFromString(clear_text))));
}

TEST_F(TigerTest, encodeEmptyTiger160)
{
   std::string hash = "3293AC630C13F0245F92BBB1766E16167A4E5849";

   EXPECT_EQ(hash, Digest::hexDigest(T2->encode(std::vector<uint8_t>(0))));
}

TEST_F(TigerTest, encodeNormalTiger192)
{
   const std::string clear_text = "The quick brown fox jumps over the lazy dog";
   std::string hash = "6D12A41E72E644F017B6F0E2F7B44C6285F06DD5D2C5B075";

   EXPECT_EQ(hash, Digest::hexDigest(T3->encode(Digest::getBytesFromString(clear_text))));
}

TEST_F(TigerTest, encodeEmptyTiger192)
{
   std::string hash = "3293AC630C13F0245F92BBB1766E16167A4E58492DDE73F3";

   EXPECT_EQ(hash, Digest::hexDigest(T3->encode(std::vector<uint8_t>(0))));
}

#endif