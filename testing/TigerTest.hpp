
#ifndef TIGERTEST_HPP
#define TIGERTEST_HPP

#include <gtest/gtest.h>
#include "../src/Tiger.hpp"

class TigerTest : public ::testing::Test
{
protected:
   Tiger *T1, *T2, *T3, *Tiger2;

   virtual void SetUp()
   {
      T1 = new Tiger(Tiger::HashSize::_128bits);
      T2 = new Tiger(Tiger::HashSize::_160bits);
      T3 = new Tiger(Tiger::HashSize::_192bits);
      Tiger2 = new Tiger(Tiger::HashSize::_192bits, Tiger::Version::tiger2);
   }

   virtual void TearDown()
   {
      delete T1;
      delete T2;
      delete T3;
      delete Tiger2;
   }
};

TEST_F(TigerTest, encodeNormalTiger128)
{
   const StringTest clear_text = "The quick brown fox jumps over the lazy dog";
   std::string hash = "6D12A41E72E644F017B6F0E2F7B44C62";

   EXPECT_EQ(hash, T1->encode(clear_text.toBytes()).toHexString());
}

TEST_F(TigerTest, encodeEmptyTiger128)
{
   std::string hash = "3293AC630C13F0245F92BBB1766E1616";

   EXPECT_EQ(hash, T1->encode(BytesVector()).toHexString());
}

TEST_F(TigerTest, encodeNormalTiger160)
{
   const StringTest clear_text = "The quick brown fox jumps over the lazy dog";
   std::string hash = "6D12A41E72E644F017B6F0E2F7B44C6285F06DD5";

   EXPECT_EQ(hash, T2->encode(clear_text.toBytes()).toHexString());
}

TEST_F(TigerTest, encodeEmptyTiger160)
{
   std::string hash = "3293AC630C13F0245F92BBB1766E16167A4E5849";

   EXPECT_EQ(hash, T2->encode(BytesVector()).toHexString());
}

TEST_F(TigerTest, encodeNormalTiger192)
{
   const StringTest clear_text = "The quick brown fox jumps over the lazy dog";
   std::string hash = "6D12A41E72E644F017B6F0E2F7B44C6285F06DD5D2C5B075";

   EXPECT_EQ(hash, T3->encode(clear_text.toBytes()).toHexString());
}

TEST_F(TigerTest, encodeEmptyTiger192)
{
   std::string hash = "3293AC630C13F0245F92BBB1766E16167A4E58492DDE73F3";

   EXPECT_EQ(hash, T3->encode(BytesVector()).toHexString());
}

TEST_F(TigerTest, encodeNormalTiger2_192)
{
   const StringTest clear_text = "The quick brown fox jumps over the lazy dog";
   std::string hash = "976ABFF8062A2E9DCEA3A1ACE966ED9C19CB85558B4976D8";

   EXPECT_EQ(hash, Tiger2->encode(clear_text.toBytes()).toHexString());
}

TEST_F(TigerTest, encodeEmptyTiger2_192)
{
   std::string hash = "4441BE75F6018773C206C22745374B924AA8313FEF919F41";

   EXPECT_EQ(hash, Tiger2->encode(BytesVector()).toHexString());
}

TEST_F(TigerTest, encodeHMACNormalText)
{
   const StringTest clear_text = "message digest";
   std::string hash = "381B28645F3AFA6C509D207DC1FCB00680C90D3A290B4391";
   const BytesVector key = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
      0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
   0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
   0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
      0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

   EXPECT_EQ(hash, T3->hmacEncode(key, clear_text.toBytes()).toHexString());
}

TEST_F(TigerTest, encodeHMACEmptyText)
{
   std::string hash = "2FA999C02ABA3644471B5A7CCF542BF61827E8F53BEA0195";
   const BytesVector key = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
      0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
   0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
   0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
      0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

   EXPECT_EQ(hash, T3->hmacEncode(key, BytesVector()).toHexString());
}

#endif