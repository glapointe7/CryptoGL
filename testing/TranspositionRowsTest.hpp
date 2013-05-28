#ifndef TRANSPOSITIONROWSTEST_HPP
#define TRANSPOSITIONROWSTEST_HPP

#include <gtest/gtest.h>
#include "../src/Transposition.hpp"

class TranspositionRowsTest : public ::testing::Test
{
protected:
   TranspositionCompleteRows *TR;

   virtual void SetUp()
   {
      TR = new TranspositionCompleteRows({7, 1, 4, 6, 2, 5, 3, 0});
   }

   virtual void TearDown()
   {
      delete TR;
   }
};

TEST_F(TranspositionRowsTest, encode)
{
   const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
   
   EXPECT_EQ("LATILNUSTLRIPAAINUUOECTQRALPIASNTGMARMAONORENIOITEBEEJOTUUEORBLSGDDIEOSTXSXXXXXT", TR->encode(clear_text));
}

TEST_F(TranspositionRowsTest, decode)
{
   const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTSXXXXXX";
   
   EXPECT_EQ(clear_text, TR->decode("LATILNUSTLRIPAAINUUOECTQRALPIASNTGMARMAONORENIOITEBEEJOTUUEORBLSGDDIEOSTXSXXXXXT"));
}

#endif