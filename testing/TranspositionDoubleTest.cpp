
#ifndef TRANSPOSITIONDOUBLETEST_HPP
#define	TRANSPOSITIONDOUBLETEST_HPP

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
   const std::string clear_text = "ATTENDONSVOSINSTRUCTIONSAVANTPOURSUIVREOPRATIONS";
   const std::vector<uint8_t> key_row = {6u, 3u, 2u, 5u, 4u, 7u, 1u};
   
   TD->setKey("FCBEDGA");
   TD->setKeyRow(key_row);
   EXPECT_EQ("PRVOEEITRTCUISIVSSONNSOPRUUTASNVANONTAOISRDTTNEOA", TD->encode(clear_text));
}

TEST_F(TranspositionDoubleTest, decode)
{
   const std::string clear_text = "ATTENDONSVOSINSTRUCTIONSAVANTPOURSUIVREOPRATIONS";
   const std::vector<uint8_t> key_row = {6u, 3u, 2u, 5u, 4u, 7u, 1u};
   
   TD->setKey("FCBEDGA");
   TD->setKeyRow(key_row);
   EXPECT_EQ(clear_text, TD->decode("PRVOEEITRTCUISIVSSONNSOPRUUTASNVANONTAOISRDTTNEOA"));
}

#endif