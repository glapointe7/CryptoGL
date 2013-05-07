#ifndef HILLTEST_HPP
#define HILLTEST_HPP

#include <gtest/gtest.h>
#include "../src/Hill.hpp"

class HillTest : public ::testing::Test
{
protected:
   Hill *H;

   virtual void SetUp()
   {
      H = new Hill();
   }

   virtual void TearDown()
   {
      delete H;
   }
};

TEST_F(HillTest, encode)
{
   const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
   const std::vector<std::vector<int32_t> > key = {{3, 4}, {5, 9}};
   
   H->setKey(key);
   EXPECT_EQ("CMJBFEQJQJTXZHWDYAKJQOQFNNSUHDJUOUZHGMYPCKRJFBMHVBQQNIKJEKRCQDSQRSGANLWQZX", H->encode(clear_text));
}

TEST_F(HillTest, decode)
{
   const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
   const std::vector<std::vector<int32_t> > key = {{3, 4}, {5, 9}};
   
   H->setKey(key);
   EXPECT_EQ(clear_text, H->decode("CMJBFEQJQJTXZHWDYAKJQOQFNNSUHDJUOUZHGMYPCKRJFBMHVBQQNIKJEKRCQDSQRSGANLWQZX"));
}

#endif