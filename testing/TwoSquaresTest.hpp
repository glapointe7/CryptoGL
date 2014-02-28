
#ifndef TWOSQUARESTEST_HPP
#define	TWOSQUARESTEST_HPP

#include <gtest/gtest.h>
#include "../src/TwoSquares.hpp"

class TwoSquaresTest : public ::testing::Test
{
protected:
   TwoSquares *P;

   virtual void SetUp()
   {
      P = new TwoSquares("ROMEO", "JULIETTE");
   }

   virtual void TearDown()
   {
      delete P;
   }
};

TEST_F(TwoSquaresTest, encode) 
{
  const ClassicalType clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
  
  EXPECT_EQ("OFGANTHOHOOCUBFCOMJFYBJPOBFVGGRPUIUBEJJGGPONIRJSRAUSTRJFOELRLFUORGJYAQGINZ", P->encode(clear_text));
}

TEST_F(TwoSquaresTest, decode) 
{
  const ClassicalType clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
  
  EXPECT_EQ(clear_text, P->decode("OFGANTHOHOOCUBFCOMJFYBJPOBFVGGRPUIUBEJJGGPONIRJSRAUSTRJFOELRLFUORGJYAQGINZ"));
}

#endif