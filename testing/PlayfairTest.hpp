
#ifndef PLAYFAIRTEST_HPP
#define	PLAYFAIRTEST_HPP

#include <gtest/gtest.h>
#include "../src/Playfair.hpp"

class PlayfairTest : public ::testing::Test
{
protected:
   Playfair *P;

   virtual void SetUp()
   {
      P = new Playfair();
   }

   virtual void TearDown()
   {
      delete P;
   }
};

TEST_F(PlayfairTest, encode) 
{
  const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
  
  P->setKey("TESTING");
  EXPECT_EQ("EBFZETNKNKOBODNEMYSEYGREEDNIHDQMMAODPUOGENREPMNSTESVAVJPSETXZRSAMVNGSIARTCEI", P->encode(clear_text));
}

TEST_F(PlayfairTest, decode) 
{
  const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
  
  P->setKey("TESTING");
  EXPECT_EQ(clear_text, P->decode("EBFZETNKNKOBODNEMYSEYGREEDNIHDQMMAODPUOGENREPMNSTESVAVJPSETXZRSAMVNGSIARTCEI"));
}

#endif