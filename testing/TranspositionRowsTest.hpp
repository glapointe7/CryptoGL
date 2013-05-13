#ifndef TRANSPOSITIONROWSTEST_HPP
#define TRANSPOSITIONROWSTEST_HPP

#include <gtest/gtest.h>
#include "../src/Transposition.hpp"

class TranspositionRowsTest : public ::testing::Test
{
protected:
   TranspositionRows *TR;

   virtual void SetUp()
   {
      TR = new TranspositionRows();
   }

   virtual void TearDown()
   {
      delete TR;
   }
};

TEST_F(TranspositionRowsTest, encode)
{
   const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
   const std::vector<uint8_t> key = {7u, 1u, 4u, 6u, 2u, 5u, 3u, 0u};
   
   TR->setKey(key);
   EXPECT_EQ("LATILNUSTLRIPAAINUUOECTQRALPIASNTGMARMAONORENIOITEBEEJOTUUEORBLSGDDIEOSTXSXXXXXT", TR->encode(clear_text));
}

TEST_F(TranspositionRowsTest, decode)
{
   const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTSXXXXXX";
   const std::vector<uint8_t> key = {7, 1, 4, 6, 2, 5, 3, 0};
   
   TR->setKey(key);
   EXPECT_EQ(clear_text, TR->decode("LATILNUSTLRIPAAINUUOECTQRALPIASNTGMARMAONORENIOITEBEEJOTUUEORBLSGDDIEOSTXSXXXXXT"));
}

#endif