
#ifndef NIHILISTESTEST_HPP
#define	NIHILISTESTEST_HPP

#include <gtest/gtest.h>
#include "../src/Nihilistes.hpp"

class NihilistesTest : public ::testing::Test
{
protected:
   Nihilistes *P;

   virtual void SetUp()
   {
      P = new Nihilistes("DIFFICILE", "EASY");
   }

   virtual void TearDown()
   {
      delete P;
   }
};

TEST_F(NihilistesTest, encode) 
{
  const ClassicalType clear_text = "LECOYOTEHURLE";
  
  EXPECT_EQ("36435889755789754673876942", P->encode(clear_text));
}

TEST_F(NihilistesTest, decode) 
{
  const ClassicalType clear_text = "LECOYOTEHURLE";
  
  EXPECT_EQ(clear_text, P->decode("36435889755789754673876942"));
}

#endif