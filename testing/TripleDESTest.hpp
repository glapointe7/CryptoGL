
#ifndef TRIPLEDESTEST_HPP
#define TRIPLEDESTEST_HPP

#include <gtest/gtest.h>
#include "../src/TripleDES.hpp"

class TripleDESTest : public ::testing::Test
{
protected:
   TripleDES *D;

   virtual void SetUp()
   {
      const BytesVector key1 = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
      const BytesVector key2 = {0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01};
      const BytesVector key3 = {0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01, 0x23};
      D = new TripleDES(key1, key2, key3);
   }

   virtual void TearDown()
   {
      delete D;
   }
};

TEST_F(TripleDESTest, encode)
{
   const StringTest clear_text = "The qufck brown fox jump";
   
   EXPECT_EQ("A826FD8CE53B855FCCE21C8112256FE668D5C05DD9B6B900",
           Vector::toHexString(D->encode(clear_text.toBytes())));
}

TEST_F(TripleDESTest, decode)
{
   const std::string clear_text = "The qufck brown fox jump";
   const StringTest message("A826FD8CE53B855FCCE21C8112256FE668D5C05DD9B6B900");
   
   EXPECT_EQ(clear_text, Vector::toString(D->decode(message.hexToBytes())));
}

#endif