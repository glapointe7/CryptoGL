
#ifndef FOURSQUARESTEST_HPP
#define	FOURSQUARESTEST_HPP

#include <gtest/gtest.h>
#include "../src/FourSquares.hpp"

class FourSquaresTest : public ::testing::Test
{
protected:
   FourSquares *P;

   virtual void SetUp()
   {
      P = new FourSquares("ROMEO", "JULIETTE");
   }

   virtual void TearDown()
   {
      delete P;
   }
};

TEST_F(FourSquaresTest, encode) 
{
  const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
  
  EXPECT_EQ("NIHVSMCKCKNJNLGQNVARXJKMHIFQHUQNIDNLJHANGKLKSBEMTEAMAAARNYPHOEHZSEERAKCCSR", P->encode(clear_text));
}

TEST_F(FourSquaresTest, decode) 
{
  const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
  
  EXPECT_EQ(clear_text, P->decode("NIHVSMCKCKNJNLGQNVARXJKMHIFQHUQNIDNLJHANGKLKSBEMTEAMAAARNYPHOEHZSEERAKCCSR"));
}

#endif