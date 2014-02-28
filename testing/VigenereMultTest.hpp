
#ifndef VIGENEREMULTTEST_HPP
#define	VIGENEREMULTTEST_HPP

#include <gtest/gtest.h>
#include "../src/Vigenere.hpp"

class VigenereMultTest : public ::testing::Test
{
protected:
   VigenereMult *V;
   
   virtual void SetUp()
   {
      V = new VigenereMult("TABARNAK");
   }

   virtual void TearDown()
   {
      delete V;
   }
};

TEST_F(VigenereMultTest, encode) 
{
  const ClassicalType clear_text = "TIGARSESTENBAWATARNACHEONDIRAIT";
  
  EXPECT_EQ("400 9 14 1 324 266 5 209 400 5 28 2 18 322 1 220 20 18 28 1 54 112 5 165 280 4 18 18 18 126 20 ", 
          V->encode(clear_text));
}

TEST_F(VigenereMultTest, decode) 
{
  const ClassicalType clear_text = "TIGARSESTENBAWATARNACHEONDIRAIT";
  
  EXPECT_EQ(clear_text, 
          V->decode("400 9 14 1 324 266 5 209 400 5 28 2 18 322 1 220 20 18 28 1 54 112 5 165 280 4 18 18 18 126 20 "));
}

#endif