#ifndef ROZIERTEST_HPP
#define ROZIERTEST_HPP

#include <gtest/gtest.h>
#include "../src/Vigenere.hpp"

class RozierTest : public ::testing::Test
{
protected:
   Rozier *R;
   
   virtual void SetUp()
   {
      R = new Rozier("TESTING");
   }

   virtual void TearDown()
   {
      delete R;
   }
};

TEST_F(RozierTest, encode) 
{
  const ClassicalType clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
  
  EXPECT_EQ("DOMJYGVWWMEFKNTHRJJMHNCOCFBFWOQGTZELANPYBBYCSXJGGPSPQOXGDISAJUBFHETXWBTUUH", R->encode(clear_text));
  
  /*R->setAlpha(String::alpha_numeric);
  R->setKey("V1Gen3rE");
  EXPECT_EQ("yPjdjBfrXjYQFXoiodUHridlWQ6prPnae4ogbkJj6ltdpRUBqkTmKZ2qyjpUUzl0ibNi1loVrb", R->encode(clear_text));
  
  R->setAlpha(String::letters);
  R->setKey("vvvvvvvv");
  EXPECT_EQ(clear_text, R->encode(clear_text));*/
}

TEST_F(RozierTest, decode) 
{
  const ClassicalType clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
  
  R->setKey("TESTING");
  EXPECT_EQ(clear_text, R->decode("DOMJYGVWWMEFKNTHRJJMHNCOCFBFWOQGTZELANPYBBYCSXJGGPSPQOXGDISAJUBFHETXWBTUUH"));
  
  /*R->setAlpha(String::alpha_numeric);
  R->setKey("V1Gen3rE");
  EXPECT_EQ(clear_text, R->decode("yPjdjBfrXjYQFXoiodUHridlWQ6prPnae4ogbkJj6ltdpRUBqkTmKZ2qyjpUUzl0ibNi1loVrb"));
  
  R->setAlpha(String::letters);
  R->setKey("vvvvvvvv");
  EXPECT_EQ(clear_text, R->decode(clear_text));*/
}

#endif