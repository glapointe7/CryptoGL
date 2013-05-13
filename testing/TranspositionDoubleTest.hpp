#ifndef TRANSPOSITIONDOUBLETEST_HPP
#define TRANSPOSITIONDOUBLETEST_HPP

#include <gtest/gtest.h>
#include "../src/Transposition.hpp"

class TranspositionDoubleTest : public ::testing::Test
{
protected:
   TranspositionDouble *TD;

   virtual void SetUp()
   {
      TD = new TranspositionDouble();
   }

   virtual void TearDown()
   {
      delete TD;
   }
};

TEST_F(TranspositionDoubleTest, encode)
{
   const std::string clear_text = "ATTENDONSVOSINSTRUCTIONSAVANTPOURSUIVREOPERATIONS";
   const std::vector<uint32_t> key_row = {5ul, 2ul, 1ul, 4ul, 3ul, 6ul, 0ul};
   const std::vector<uint8_t> key = {5u, 2u, 1u, 4u, 3u, 6u, 0u};
   
   TD->setKey(key);
   TD->setKeyRow(key_row);
   EXPECT_EQ("PRVOEEITRTCUISIVSSONNSOPRUUTASNVANONTAOISRDTTNEOA", TD->encode(clear_text));
}

TEST_F(TranspositionDoubleTest, decode)
{
   const std::string clear_text = "ATTENDONSVOSINSTRUCTIONSAVANTPOURSUIVREOPERATIONS";
   const std::vector<uint32_t> key_row = {5ul, 2ul, 1ul, 4ul, 3ul, 6ul, 0ul};
   const std::vector<uint8_t> key = {5u, 2u, 1u, 4u, 3u, 6u, 0u};
   
   TD->setKey(key);
   TD->setKeyRow(key_row);
   EXPECT_EQ(clear_text, TD->decode("PRVOEEITRTCUISIVSSONNSOPRUUTASNVANONTAOISRDTTNEOA"));
}

#endif