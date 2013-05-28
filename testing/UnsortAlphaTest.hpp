#ifndef UNSORTALPHATEST_HPP
#define UNSORTALPHATEST_HPP

#include <gtest/gtest.h>
#include "../src/UnsortAlpha.hpp"
#include "../src/String.hpp"

class UnsortAlphaTest : public ::testing::Test
{
protected:
   UnsortAlpha *UA;

   virtual void SetUp()
   {
      UA = new UnsortAlpha("TESTING");
   }

   virtual void TearDown()
   {
      delete UA;
   }
};

TEST_F(UnsortAlphaTest, encode_horiz)
{
   const std::string clear_text = "ATTENDONSVOSINSTRUCTIONSAVANTPOURSUIVREOPRATIONS";
   
   UA->setHorizontalAlpha();
   EXPECT_EQ("TRRNKILKQVLQCKQRPUSRCLKQTVTKRMLUPQUCVPNLMPTRCLKQ", UA->encode(clear_text));
}

TEST_F(UnsortAlphaTest, encode_vert)
{
   const std::string clear_text = "ATTENDONSVOSINSTRUCTIONSAVANTPOURSUIVREOPRATIONS";
   
   UA->setKey("TESTING");
   UA->setVerticalAlpha();
   EXPECT_EQ("TFFYUQIUNWINRUNFVOJFRIUNTWTUFDIOVNORWVYIDVTFRIUN", UA->encode(clear_text));
}

TEST_F(UnsortAlphaTest, decode_horiz)
{
   const std::string clear_text = "ATTENDONSVOSINSTRUCTIONSAVANTPOURSUIVREOPRATIONS";
   
   UA->setKey("TESTING");
   UA->setHorizontalAlpha();
   EXPECT_EQ(clear_text, UA->decode("TRRNKILKQVLQCKQRPUSRCLKQTVTKRMLUPQUCVPNLMPTRCLKQ"));
}

TEST_F(UnsortAlphaTest, decode_vert)
{
   const std::string clear_text = "ATTENDONSVOSINSTRUCTIONSAVANTPOURSUIVREOPRATIONS";
   
   UA->setKey("TESTING");
   UA->setVerticalAlpha();
   EXPECT_EQ(clear_text, UA->decode("TFFYUQIUNWINRUNFVOJFRIUNTWTUFDIOVNORWVYIDVTFRIUN"));
}

#endif