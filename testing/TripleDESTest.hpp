
#ifndef TRIPLEDESTEST_HPP
#define	TRIPLEDESTEST_HPP

#include <gtest/gtest.h>
#include "../src/TripleDES.hpp"

class TripleDESTest : public ::testing::Test
{
protected:
   TripleDES *D;

   virtual void SetUp()
   {
      D = new TripleDES();
   }

   virtual void TearDown()
   {
      delete D;
   }
};

TEST_F(TripleDESTest, encode)
{
   const std::string clear_text = "The qufck brown fox jump";
   const std::vector<uint8_t> key = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
   const std::vector<uint8_t> key2 = {0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01};
   const std::vector<uint8_t> key3 = {0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01, 0x23};
   
   D->setKey(key);
   D->setSecondKey(key2);
   D->setThirdKey(key3);
   EXPECT_EQ("A826FD8CE53B855FCCE21C8112256FE668D5C05DD9B6B900", 
           TripleDES::hexDigest(D->encode(TripleDES::getBytesFromString(clear_text))));
}

TEST_F(TripleDESTest, decode)
{
   const std::string clear_text = "The qufck brown fox jump";
   const std::vector<uint8_t> key = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
   const std::vector<uint8_t> key2 = {0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01};
   const std::vector<uint8_t> key3 = {0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01, 0x23};
   
   D->setKey(key);
   D->setSecondKey(key2);
   D->setThirdKey(key3);
   EXPECT_EQ(clear_text, 
           TripleDES::getStringFromBytes(D->decode(TripleDES::getBytesFromHexDigest("A826FD8CE53B855FCCE21C8112256FE668D5C05DD9B6B900"))));
}

#endif