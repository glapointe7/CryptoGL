
#ifndef MD2TEST_HPP
#define MD2TEST_HPP

#include <gtest/gtest.h>
#include "../src/MD2.hpp"

class MD2Test : public ::testing::Test
{
protected:
   MD2 *S;

   virtual void SetUp()
   {
      S = new MD2();
   }

   virtual void TearDown()
   {
      delete S;
   }
};

TEST_F(MD2Test, encodeNormalText)
{
   const StringTest clear_text = "message digest";
   std::string hash = "AB4F496BFB2A530B219FF33031FE06B0";

   EXPECT_EQ(hash, S->encode(clear_text.toBytes()).toHexString());
}

TEST_F(MD2Test, encodeEmptyText)
{
   std::string hash = "8350E5A3E24C153DF2275C9F80692773";

   EXPECT_EQ(hash, S->encode(BytesVector()).toHexString());
}

#endif